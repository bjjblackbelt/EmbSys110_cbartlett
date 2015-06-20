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
#include "OS.h"
#include "IUart.h"

// Global objects
namespace Bsp {
extern IUart* g_pUart;
}

#if ENABLE_CPU_USAGE_MEAS
namespace OS {
extern State_t g_os;
}
#endif

void Thread::Idle(void* pData)
{
    if (pData == NULL) { return; }

    while (1)
    {
#if ENABLE_CPU_USAGE_MEAS
        OS::g_os.idleCtr++;
#endif
    }
}

void Thread::Thread1(void* pData)
{
    if (pData == NULL) { return; }

    while (1)
    {
        GlobalData_t* s = static_cast<GlobalData_t*>(pData);

        s->nT1++;

        //!< Obtain critical section lock and increment shared counter
        if (OS::EnterCS(s->guard) == CriticalSection::SUCCESS)
        {
            s->inc++;
            OS::LeaveCS(s->guard);
        }

        Bsp::TglLed(Bsp::PIN_LED_GREEN);

        OS::TimeDlyMs(500);
    }
}

void Thread::Thread2(void* pData)
{
    if (pData == NULL) { return; }

    while (1)
    {
        GlobalData_t* s = static_cast<GlobalData_t*>(pData);

        s->nT2++;

        //!< Obtain critical section lock and decrement shared counter
        if (OS::EnterCS(s->guard) == CriticalSection::SUCCESS)
        {
            s->inc--;
            OS::LeaveCS(s->guard);
        }

        Bsp::TglLed(Bsp::PIN_LED_BLUE);

        OS::TimeDlyMs(500);
    }
}

void Thread::Thread3(void* pData)
{

    if (pData == NULL) { return; }

    while (1)
    {
        GlobalData_t* s = static_cast<GlobalData_t*>(pData);

        //!< Obtain critical section lock and check shared counter
        if (OS::EnterCS(s->guard) == CriticalSection::SUCCESS)
        {
            if (s->inc > 2)
            {
                __asm volatile("cpsid i");

                // ERROR: Reset the counter and continue
                s->inc = 0;

                Bsp::SetLed(Bsp::PIN_LED_GREEN);
                Bsp::SetLed(Bsp::PIN_LED_BLUE);

                for (int i = 0; i < 30; i++)
                {
                    Bsp::TglLed(Bsp::PIN_LED_GREEN);
                    Bsp::TglLed(Bsp::PIN_LED_BLUE);
                    uint32_t cnt = 0;
                    while (cnt++ < 100000);
                }

                __asm volatile("cpsie i");
            }
            OS::LeaveCS(s->guard);
        }

        OS::TimeDlyMs(777);
    }
}
