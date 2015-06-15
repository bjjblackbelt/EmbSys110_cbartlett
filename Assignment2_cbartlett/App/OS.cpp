/**
 *******************************************************************************
 *  @file   OS.cpp
 *  @brief  Implementation of the OS class.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "OS.h"
#include "Bsp.h"
#include "Threads.h"
#include "TimingMeasurements.h"
#include "IUart.h"

extern "C" {
#include <stm32f10x_rcc.h>
}

// Initialize globals
namespace OS {
State_t g_os;
}

// Global objects
namespace Bsp {
extern IUart* g_pUart;
}

OS::Error_t OS::RegisterThread(Thread::Thread_t& thread, uint32_t stackSize)
{
    TIME_MEAS_CREATE_TASK_START();
    OS::Error_t status = OS::ERROR_NONE;

    if (OS::g_os.nThreads > (OS::MAX_THREAD_COUNT - 1))
    {
        status = OS::ERROR_MAX_THREADS_REGISTERED;
        goto RegisterCleanup;
    }

    if (thread.entry == NULL)
    {
        status = OS::ERROR_NULL;
        goto RegisterCleanup;
    }

    if (thread.stack == NULL)
    {
        status = OS::ERROR_NULL;
        goto RegisterCleanup;
    }

    // Initialize task stack
    OS::InitializeThreadStack(thread, stackSize);

    // Update OS State
    OS::g_os.threadQueue[OS::g_os.nThreads]        = &thread;
    OS::g_os.threadQueue[OS::g_os.nThreads]->state = Thread::STATE_READY;
    OS::g_os.threadDelay[OS::g_os.nThreads][0]     = false;
    OS::g_os.threadDelay[OS::g_os.nThreads][1]     = UINT32_MAX;

    // Update thread count
    OS::g_os.nThreads++;

RegisterCleanup:
    TIME_MEAS_CREATE_TASK_STOP();
    return status;
}

void OS::SetNextReadyThread()
{
    // Find the next READY thread; start at the current thread + 1.
    bool isReadyThread = false;                                         //!< Indicates if any threads were READY
    uint_fast8_t nThreadCnt = 1;                                        //!< Go through every thread but Idle
    uint_fast8_t nextThread = (uint_fast8_t)OS::g_os.currThread + 1U;   //!< Start at the current thread + 1
    while ((nThreadCnt < OS::g_os.nThreads) && (isReadyThread == false))
    {
        nThreadCnt++;

        // If at the end of the thread queue, wrap to beginning.
        if (nextThread >= (OS::g_os.nThreads))
        {
            nextThread = 1;
        }

        if (OS::g_os.threadQueue[nextThread]->state == Thread::STATE_READY)
        {
            OS::g_os.threadQueue[nextThread]->state = Thread::STATE_ACTIVE;
            isReadyThread = true;
            OS::g_os.nextThread = nextThread;
        }

        nextThread++;
    }

    // If no threads were READY, then set the next thread to the Idle thread.
    if (isReadyThread == false)
    {
        OS::g_os.nextThread = OS::UID_THREAD_IDLE;
    }
}

void OS::PrintThreadInfo()
{
    if (OS::g_os.nThreads > 0)
    {
        Bsp::g_pUart->PrintStr("Name: ");
        Bsp::g_pUart->PrintStr(OS::g_os.threadQueue[OS::g_os.currThread]->name);
        Bsp::g_pUart->PrintStr("\n");
        Bsp::g_pUart->PrintStr("State: ");
        Bsp::g_pUart->PrintHex(OS::g_os.threadQueue[OS::g_os.currThread]->state);
        Bsp::g_pUart->PrintStr("\n");
    }
    else
    {
        Bsp::g_pUart->PrintStr("There are no registered threads.\n");
    }
}

CriticalSection::Status_t OS::EnterCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Query(OS::g_os.currThread);

    if (status == CriticalSection::BUSY)
    {
        OS::g_os.threadQueue[OS::g_os.currThread]->state = Thread::STATE_BLOCKED;

        status = cs.Enter(OS::g_os.currThread);
    }
    OS::g_os.threadQueue[OS::g_os.currThread]->state = Thread::STATE_READY;

    return (status);
}

CriticalSection::Status_t OS::LeaveCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Leave(OS::g_os.currThread);
#if 0
    if (status == CriticalSection::SUCCESS)
    {
        for (uint8_t thread = 1; thread < OS::g_os.nThreads; thread++)
        {
            if (OS::g_os.threadQueue[thread]->state == Thread::STATE_BLOCKED)
            {
                OS::g_os.threadQueue[thread]->state = Thread::STATE_READY;
            }
        }
    }
#endif

    return (status);
}

CriticalSection::Status_t OS::QueryCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Query(OS::g_os.currThread);
    return (status);
}

void OS::Start(uint32_t* topOfIdleStack)
{
    if (OS::g_os.nThreads > 0)
    {
        // Set current task to Idle
        OS::g_os.currThread = OS::UID_THREAD_IDLE;

        // Update the process stack pointer
        __asm volatile(
            "msr psp, %0;"
            :
            : "r"((uint32_t)topOfIdleStack)
        );

        // set PendSV to the lowest priority
        NVIC_SetPriority(PendSV_IRQn, 0xFF);

        // Configure SysTick Timer
        if (SysTick_Config(Bsp::SYS_TICKS_BETWEEN_SYSTICK_IRQ)) while (1);

        // Switch to the process stack and user mode
        __asm volatile(
            "msr control, %0"
            :
            : "r"(0x3)
        );

        // Flush the pipeline after changing CONTROL (recommended for Cortex-M3)
        __asm volatile("isb");

        // Start in idle
        OS::g_os.threadQueue[OS::g_os.currThread]->entry(OS::g_os.threadQueue[OS::g_os.currThread]->data);

        while (1)
        {
            /* Never reach this. */;
        }
    }
}

void OS::InitializeThreadStack(Thread::Thread_t& thread, uint32_t stackSize)
{
    //!< EXC_RETURN values
    static const uint32_t PROC_RETURN = 0xFFFFFFFD;

    // Fill task stack with pattern
    for (uint32_t word = 0; word < (stackSize - 1); word++)
    {
        *(thread.stack + word) = 0xDEADBEEF;
    }

    //!< Initialize the default stack frame
    uint32_t* defaultFrame = thread.stack + stackSize - 1;

    /* Registers stacked as if saved on exception    */
    *(defaultFrame)  = (uint32_t)0x01000000L;             /* xPSR                                          */
    *(--defaultFrame)  = (uint32_t)thread.entry;            /* Entry Point                                   */
    *(--defaultFrame)  = (uint32_t)PROC_RETURN;             /* LR                                           */
    *(--defaultFrame)  = (uint32_t)0x12121212L;             /* R12                                           */
    *(--defaultFrame)  = (uint32_t)0x03030303L;             /* R3                                            */
    *(--defaultFrame)  = (uint32_t)0x02020202L;             /* R2                                            */
    *(--defaultFrame)  = (uint32_t)0x01010101L;             /* R1                                            */
    *(--defaultFrame)  = (uint32_t)thread.data;             /* R0 : argument                                 */

    /* Remaining registers saved on process stack    */
    *(--defaultFrame)  = (uint32_t)0x11111111L;             /* R11                                           */
    *(--defaultFrame)  = (uint32_t)0x10101010L;             /* R10                                           */
    *(--defaultFrame)  = (uint32_t)0x09090909L;             /* R9                                            */
    *(--defaultFrame)  = (uint32_t)0x08080808L;             /* R8                                            */
    *(--defaultFrame)  = (uint32_t)0x07070707L;             /* R7                                            */
    *(--defaultFrame)  = (uint32_t)0x06060606L;             /* R6                                            */
    *(--defaultFrame)  = (uint32_t)0x05050505L;             /* R5                                            */
    *(--defaultFrame)  = (uint32_t)0x04040404L;             /* R4                                            */

    thread.stack = defaultFrame;
}

void OS::TimeDlyMs(uint32_t timeMs)
{
    OS::g_os.threadQueue[OS::g_os.currThread]->state = Thread::STATE_DELAYED;
    OS::g_os.threadDelay[OS::g_os.currThread][1] = Bsp::GetSysTick() + TIME_MS_TO_TICK(timeMs);
    OS::g_os.threadDelay[OS::g_os.currThread][0] = true;
    while (OS::g_os.threadDelay[OS::g_os.currThread][0] == 1);
}

void OS::TimeTick()
{
    for (uint8_t thread = 1; thread < OS::g_os.nThreads; thread++)
    {
        if (OS::g_os.threadDelay[thread][1] <= Bsp::GetSysTick())
        {
            // Reset threadDelay
            OS::g_os.threadDelay[thread][1] = UINT32_MAX;

            // Reset delay indicator
            OS::g_os.threadDelay[thread][0] = false;

            // Change state
            OS::g_os.threadQueue[thread]->state = Thread::STATE_READY;
        }
    }
}
