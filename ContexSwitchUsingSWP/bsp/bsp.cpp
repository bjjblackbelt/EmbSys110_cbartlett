/**
 *******************************************************************************
 *  @filename   bsp.cpp
 *  @brief      Implements board specific functions
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "bsp.h"

/*
********************************************************************************
* Uses SWP instruction to set the lock for a critical section.
*
* @param lock - A reference to the lock variable
* @returns Returns the value of LOCKED_STATE if the lock was NOT acquired.
********************************************************************************
*/
CSStatus_t BSP_CSLock(int& lock)
{
	CSStatus_t lockStatus;

	asm volatile ("MOV r1, %2;"
				  "MOV r2, %1;"
				  "SWP %0, r1, [r2];"
			      : "=r" (lockStatus) 					/* Outputs: %0     */
				  : "r" (&lock), "I" (LOCKED_STATE)    	/* Inputs:  %1, %2 */
                  : "r1", "r2"                          /* Clobbered Regs  */
				  );

    return lockStatus;
}
