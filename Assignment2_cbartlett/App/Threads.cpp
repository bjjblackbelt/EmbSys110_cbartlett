/**
 *******************************************************************************
 *  @file   Threads.cpp
 *  @brief  Implementations for the threads of this application.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#include <stddef.h>
#include "Threads.h"
#include "Bsp.h"
#include "IUart.h"

// Global objects
namespace Bsp {
extern IUart* g_pUart;
extern OS* g_pOS;
}

// Prototypes
static void PrintExecutionTime(char const * const msg, uint_fast32_t time);
static void PrintSharedDataState(Thread::GlobalData_t& s);

#define ENABLE_THREAD_EXECUTION_TIME_PRINT 1
#ifdef ENABLE_THREAD_EXECUTION_TIME_PRINT
#define PRINT_EXEC_TIME(msg,time)       PrintExecutionTime((msg),(time))
#else
#define PRINT_EXEC_TIME(msg,time)
#endif

Thread::Error_t Thread::Idle(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    uint_fast32_t ticks = Bsp::GetSysTick();

    Bsp::ClrLed(Bsp::PIN_LED_GREEN);

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    Bsp::DelayMs(10000);

    //!< Print state of shared data
    PrintSharedDataState(*s);

    //!< Print Execution Time
    PRINT_EXEC_TIME("> Idle exec time: ", TIME_TICK_TO_MS(Bsp::GetSysTick() - ticks));

    Bsp::SetLed(Bsp::PIN_LED_GREEN);

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread1(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    uint_fast32_t ticks = Bsp::GetSysTick();

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    s->nT1++;

    //!< Obtain critical section lock and increment shared counter
    if (Bsp::g_pOS->EnterCS(s->guard) == CriticalSection::SUCCESS)
    {
        s->inc++;
        Bsp::g_pOS->LeaveCS(s->guard);
    }

    Bsp::DelayMs(1000);

    //!< Print Execution Time
    PRINT_EXEC_TIME("> Thread1 exec time: ", TIME_TICK_TO_MS(Bsp::GetSysTick() - ticks));

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread2(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    uint_fast32_t ticks = Bsp::GetSysTick();

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    s->nT2++;

    //!< Obtain critical section lock and decrement shared counter
    if (Bsp::g_pOS->EnterCS(s->guard) == CriticalSection::SUCCESS)
    {
        s->inc--;
        Bsp::g_pOS->LeaveCS(s->guard);
    }

    Bsp::DelayMs(1000);

    //!< Print Execution Time
    PRINT_EXEC_TIME("> Thread2 exec time: ", TIME_TICK_TO_MS(Bsp::GetSysTick() - ticks));

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread3(void* pData)
{

    if (pData == NULL) return (Thread::ERROR_NULL_DATA);


    uint_fast32_t ticks = Bsp::GetSysTick();

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    //!< Obtain critical section lock and check shared counter
    Thread::Error_t status = Thread::ERROR_COUNTER;
    if (Bsp::g_pOS->EnterCS(s->guard) == CriticalSection::SUCCESS)
    {
        if (s->inc < 3)
        {
            status = Thread::ERROR_NONE;
        }
        else
        {
            // Reset the counter and continue
            s->inc = 0;
        }
        Bsp::g_pOS->LeaveCS(s->guard);
    }

    Bsp::DelayMs(1000);

    //!< Print Execution Time
    PRINT_EXEC_TIME("> Thread3 exec time: ", TIME_TICK_TO_MS(Bsp::GetSysTick() - ticks));

    return (status);
}

static void PrintExecutionTime(char const * const msg, uint_fast32_t time)
{
    Bsp::g_pUart->PrintStr(msg);
    Bsp::g_pUart->PrintUInt(time);
    Bsp::g_pUart->PrintStr("-ms\n");
}

static void PrintSharedDataState(Thread::GlobalData_t& s)
{
    Bsp::g_pUart->PrintStr("Inc: ");
    Bsp::g_pUart->PrintUInt(s.inc);
    Bsp::g_pUart->PrintStr("\n");
    Bsp::g_pUart->PrintStr("nT1: ");
    Bsp::g_pUart->PrintUInt(s.nT1);
    Bsp::g_pUart->PrintStr("\n");
    Bsp::g_pUart->PrintStr("nT2: ");
    Bsp::g_pUart->PrintUInt(s.nT2);
    Bsp::g_pUart->PrintStr("\n");
}
