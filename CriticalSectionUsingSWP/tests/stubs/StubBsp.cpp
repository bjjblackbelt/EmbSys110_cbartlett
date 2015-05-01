/**
 *******************************************************************************
 *  @filename   StubBsp.cpp
 *  @brief      Stubs out functionality for bsp.cpp.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "bsp.h"

CSStatus_t BSP_CSLock(int& lock)
{
	CSStatus_t temp = static_cast<CSStatus_t>(lock);
	lock = LOCKED_STATE;

    return temp;
}
