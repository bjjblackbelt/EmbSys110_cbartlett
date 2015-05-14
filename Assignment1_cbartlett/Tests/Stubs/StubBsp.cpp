/**
 *******************************************************************************
 *  @filename   StubBsp.cpp
 *  @brief      Stubs out functionality for bsp.cpp.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "Bsp.h"

 #define SYSCLK_FREQ_24MHz  24000000
uint32_t SystemCoreClock = SYSCLK_FREQ_24MHz;

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
