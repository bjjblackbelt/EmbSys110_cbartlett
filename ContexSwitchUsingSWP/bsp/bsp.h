/**
 *******************************************************************************
 *  @filename   bsp.h
 *  @brief      See implementation file for description.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#ifndef BSP_H
#define BSP_H

typedef enum
{
    UNLOCKED_STATE = 0,
    LOCKED_STATE   = 1
} CSStatus_t;

CSStatus_t BSP_CSLock(int& lock);

#endif // #ifndef BSP_H
