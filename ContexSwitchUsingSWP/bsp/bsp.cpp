
#include "lpc2378.h"
#include <stdint.h>
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
	int lockStatus;

	asm volatile ("MOV r1, %2;"
				  "LDR r2, [%1];"
				  "SWP %0, r1, [r2]"
			      : "=r" (lockStatus) 					/* Outputs */
				  : "r" (lock), "r" (LOCKED_STATE)    	/* Inputs */
				  );

    return (static_cast<CSStatus_t>(lockStatus));
}
