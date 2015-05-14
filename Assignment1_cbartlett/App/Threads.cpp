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
}

Thread::Error_t Thread::Idle(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

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
    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread1(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread2(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    return Thread::ERROR_NONE;
}

Thread::Error_t Thread::Thread3(void* pData)
{
    if (pData == NULL) return (Thread::ERROR_NULL_DATA);

    GlobalData_t* s = static_cast<GlobalData_t*>(pData);

    return Thread::ERROR_NONE;
}
