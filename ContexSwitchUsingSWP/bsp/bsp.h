/**
 *******************************************************************************
 *  @filename   bsp.h
 *  @brief      See implementation file for description.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#ifndef BSP_H
#define BSP_H

#include <stdint.h>

typedef enum
{
    UNLOCKED_STATE = 0,
    LOCKED_STATE   = 1
} CSStatus_t;

CSStatus_t BSP_CSLock(int& lock);
void BSP_InitHardware();
void BSP_InitUART0(uint16_t baud, uint8_t mode, uint8_t fmode);
void BSP_PinConfigurator();
void handlerDummy();

#endif // #ifndef BSP_H
