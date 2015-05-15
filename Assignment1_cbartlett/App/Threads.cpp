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

Thread::Error_t Thread::Idle(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    uint_fast32_t ticks = Bsp::GetSysTick();

    Bsp::ClrLed(Bsp::PIN_LED_GREEN);

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    Bsp::DelayMs(10000);

    Bsp::g_pUart->PrintStr("Inc: ");
    Bsp::g_pUart->PrintUInt(s->inc);
    Bsp::g_pUart->PrintStr("\n");
    Bsp::g_pUart->PrintStr("nT1: ");
    Bsp::g_pUart->PrintUInt(s->nT1);
    Bsp::g_pUart->PrintStr("\n");
    Bsp::g_pUart->PrintStr("nT2: ");
    Bsp::g_pUart->PrintUInt(s->nT2);
    Bsp::g_pUart->PrintStr("\n");

    Bsp::SetLed(Bsp::PIN_LED_GREEN);

    Bsp::g_pUart->PrintStr("> Idle exec time: ");
    Bsp::g_pUart->PrintUInt(TIME_TICK_TO_MS(Bsp::GetSysTick()) - ticks);
    Bsp::g_pUart->PrintStr("-ms\n");

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread1(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    uint_fast32_t ticks = Bsp::GetSysTick();

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    s->nT1++;

    if (Bsp::g_pOS->EnterCS(s->guard) == CriticalSection::SUCCESS)
    {
        s->inc++;
        Bsp::g_pOS->LeaveCS(s->guard);
    }

    Bsp::g_pUart->PrintStr("> Thread1 exec time: ");
    Bsp::g_pUart->PrintUInt(TIME_TICK_TO_MS(Bsp::GetSysTick()) - ticks);
    Bsp::g_pUart->PrintStr("-ms\n");

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread2(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    uint_fast32_t ticks = Bsp::GetSysTick();

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    s->nT2++;

    if (Bsp::g_pOS->EnterCS(s->guard) == CriticalSection::SUCCESS)
    {
        s->inc--;
        Bsp::g_pOS->LeaveCS(s->guard);
    }

    Bsp::g_pUart->PrintStr("> Thread2 exec time: ");
    Bsp::g_pUart->PrintUInt(TIME_TICK_TO_MS(Bsp::GetSysTick()) - ticks);
    Bsp::g_pUart->PrintStr("-ms\n");

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread3(void* pData)
{

    if (pData == NULL) return (Thread::ERROR_NULL_DATA);


    uint_fast32_t ticks = Bsp::GetSysTick();

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    Thread::Error_t status = Thread::ERROR_COUNTER;
    if (Bsp::g_pOS->EnterCS(s->guard) == CriticalSection::SUCCESS)
    {
        if (s->inc < 3)
        {
            status = Thread::ERROR_NONE;
        }
        Bsp::g_pOS->LeaveCS(s->guard);
    }

    Bsp::g_pUart->PrintStr("> Thread3 exec time: ");
    Bsp::g_pUart->PrintUInt(TIME_TICK_TO_MS(Bsp::GetSysTick()) - ticks);
    Bsp::g_pUart->PrintStr("-ms\n");


    return (status);
}
