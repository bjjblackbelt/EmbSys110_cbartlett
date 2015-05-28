/**
 *******************************************************************************
 *  @file   OS.cpp
 *  @brief  Implementation of the OS class.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#include <stddef.h>
#include <string.h>
#include "OS.h"
#include "Threads.h"
#include "IUart.h"
#include "DTimer.h"

#define ENABLE_STATE_CHANGE_PRINTING 1
#if ENABLE_STATE_CHANGE_PRINTING
#define DEBUG_PRINT_STATE_CHANGE()      PrintThreadInfo()
#else
#define DEBUG_PRINT_STATE_CHANGE()
#endif

// We know what we're doing here!!!
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"

OS::OS(IUart& uart, DTimer& timer)
    : m_uart(&uart),
      m_timer(&timer),
      m_threadQueue(),
      m_threadStacks(),
      m_currThread(0),
      m_nThreads(0)
{
    uint32_t* addr = (uint32_t*)&DTimer::TIM6_DAC_IRQHandler;
    (void)addr;
    for (int i = 0; i < OS::MAX_THREAD_COUNT; ++i)
    {
        m_threadQueue[i] = NULL;

        for (uint32_t word = 0; word < OS::Stack_t::THREAD_STACK_SIZE_WORDS; word++)
        {
            m_threadStacks[i].stack[word] = 0xC0DE;
        }

        m_threadStacks[i].bot = &m_threadStacks[i].stack[0];
        m_threadStacks[i].top = &m_threadStacks[i].stack[OS::Stack_t::THREAD_STACK_SIZE_WORDS-1];
        m_threadStacks[i].cur = m_threadStacks[i].top;
    }

    m_uart->Init();
}

OS::~OS()
{
}

OS::Error_t OS::RegisterThread(Thread::Thread_t& thread)
{
    OS::Error_t status = OS::ERROR_NONE;

    if (m_nThreads > (OS::MAX_THREAD_COUNT - 1))
    {
        status = OS::ERROR_MAX_THREADS_REGISTERED;
        goto RegisterCleanup;
    }

    if (thread.entry == NULL)
    {
        status = OS::ERROR_NULL;
        goto RegisterCleanup;
    }

    for (uint_fast8_t i = 0; i < m_nThreads; ++i)
    {
        if (m_threadQueue[i]->uid == thread.uid)
        {
            status = OS::ERROR_UID_NOT_UNIQUE;
            goto RegisterCleanup;
        }
    }

    m_threadQueue[m_nThreads++] = &thread;

RegisterCleanup:
    return status;
}

void OS::SetNextReadyThread()
{
    // Find the next READY thread; start at the current thread + 1.
    bool isReadyThread = false;     //!< Indicates if any threads were READY
    uint_fast8_t nThreadCnt = 1;    //!< Start at thread 1; do not count Idle thread.
    while ((nThreadCnt < m_nThreads) && (isReadyThread == false))
    {
        nThreadCnt++;

        // If at the end of the thread queue, wrap to beginning.
        if (m_currThread == (m_nThreads - 1))
        {
            m_currThread = 1;
        }
        else
        {
            m_currThread++;
        }

        if (m_threadQueue[m_currThread]->state == Thread::STATE_READY)
        {
            m_threadQueue[m_currThread]->state = Thread::STATE_ACTIVE;
            DEBUG_PRINT_STATE_CHANGE();
            isReadyThread = true;
        }
    }

    // If no threads were READY, then set the next thread to the Idle thread.
    if (isReadyThread == false)
    {
        m_currThread = 0;
    }
}

void OS::PrintThreadInfo()
{
    if (m_nThreads > 0)
    {
        m_uart->PrintStr("Name: ");
        m_uart->PrintStr(m_threadQueue[m_currThread]->name);
        m_uart->PrintStr("\n");
        m_uart->PrintStr("State: ");
        m_uart->PrintHex(m_threadQueue[m_currThread]->state);
        m_uart->PrintStr("\n");
    }
    else
    {
        m_uart->PrintStr("There are no registered threads.\n");
    }
}

CriticalSection::Status_t OS::EnterCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Query(m_threadQueue[m_currThread]->uid);

    if (status == CriticalSection::BUSY)
    {
        m_threadQueue[m_currThread]->state = Thread::STATE_BLOCKED;
        DEBUG_PRINT_STATE_CHANGE();

        status = cs.Enter(m_threadQueue[m_currThread]->uid);
    }

    m_threadQueue[m_currThread]->state = Thread::STATE_READY;
    DEBUG_PRINT_STATE_CHANGE();

    return (status);
}

CriticalSection::Status_t OS::LeaveCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Leave(m_threadQueue[m_currThread]->uid);
    return (status);
}

CriticalSection::Status_t OS::QueryCS(CriticalSection& cs)
{
    CriticalSection::Status_t status = cs.Query(m_threadQueue[m_currThread]->uid);
    return (status);
}

void OS::Start()
{
    if (m_nThreads > 0)
    {
        for (uint_fast8_t i = 0; i < m_nThreads; ++i)
        {
            m_currThread = i;
            m_threadQueue[m_currThread]->state = Thread::STATE_READY;
            DEBUG_PRINT_STATE_CHANGE();

            // Initialize the individual thread stacks
            uint32_t addr = (uint32_t)(m_threadQueue[i]->entry);
            (void)addr;
            *m_threadStacks[i].cur-- = (uint32_t)(m_threadQueue[i]->entry);
        }

        // Start task timer
        // Execute the first task
        m_currThread = 0;
        m_threadQueue[m_currThread]->state = Thread::STATE_ACTIVE;
        DEBUG_PRINT_STATE_CHANGE();

        // Change the current stack pointer to that of the Idle task
        uint32_t* sp = m_threadStacks[m_currThread].cur;
        asm volatile ("MOV      sp, %[currentSP];"
                      :                        /* Outputs */
                      : [currentSP] "r" (sp)   /* Inputs */
                      :                        /* Clobbered Regs  */
                      );

        // Start OS timer
        m_timer->Open();
        m_timer->Start();
        m_threadQueue[m_currThread]->entry(m_threadQueue[m_currThread]->data);

        while (1)
        {
        }
    }
}

int_fast8_t OS::GetCurrentThreadID()
{
    if (m_nThreads > 0)
    {
        return static_cast<int_fast8_t>(m_threadQueue[m_currThread]->uid);
    }
    else
    {
        return (-1);
    }
}

uint_fast8_t OS::GetNumberOfThreads()
{
    return m_nThreads;
}

#pragma GCC diagnostic pop
