/**
 *******************************************************************************
 *  @filename   StubBsp.cpp
 *  @brief      Stubs out functionality for bsp.cpp.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "Bsp.h"
#include "StubDUart.h"

 #define SYSCLK_FREQ_24MHz  24000000
uint32_t SystemCoreClock = SYSCLK_FREQ_24MHz;

namespace Bsp {
static StubDUart s_stubUart;
IUart* g_pUart = &s_stubUart;
}

Bsp::ButtonState_t Bsp::ReadUserBtn()
{
    return Bsp::BTN_RELEASED;
}

void Bsp::SetLed(Bsp::PinConfiguration_t led)
{
    (void)led;
}

void Bsp::ClrLed(Bsp::PinConfiguration_t led)
{
    (void)led;
}

void Bsp::TglLed(Bsp::PinConfiguration_t led)
{
    (void)led;
}

uint32_t Bsp::GetSysTick()
{
    return 0;
}

Bsp::CSStatus_t Bsp::CSLock(int* lock)
{
	Bsp::CSStatus_t temp = static_cast<Bsp::CSStatus_t>(*lock);
	*lock = Bsp::LOCKED_STATE;

    return temp;
}

void Bsp::DelayMs(uint32_t nTime)
{
    (void)nTime;
}
