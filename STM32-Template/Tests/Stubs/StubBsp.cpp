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

App::CSStatus_t App::CSLock(int* lock)
{
	App::CSStatus_t temp = static_cast<App::CSStatus_t>(*lock);
	*lock = App::LOCKED_STATE;

    return temp;
}
