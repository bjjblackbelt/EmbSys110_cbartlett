/**
 *******************************************************************************
 *  @file   Bsp.h
 *  @brief  Defines a set of board specific functions.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#ifndef BSP_H
#define BSP_H

#include <stdint.h>

extern "C" {
#include <stm32f10x.h>
#include <system_stm32f10x.h>
#include <stm32f10x_gpio.h>
}

// Forward declarations
class DUart;

#define TIME_MS_TO_TICK(ms)     (((ms)*Bsp::SYS_SYSTICK_PER_SEC)/1000)
#define TIME_TICK_TO_MS(tick)   ((1000*(tick))/Bsp::SYS_SYSTICK_PER_SEC)

namespace Bsp {
/**
 * @defgroup APP_CONSTANTS Bsp Constants
 * @brief Global constants used within this application.
 * @{
 */
//

//!< The number of clock cycles between calls to SysTick_Handler
static const uint32_t SYS_TICKS_BETWEEN_SYSTICK_IRQ = SYSCLK_FREQ_24MHz / 2000;
static const uint32_t SYS_SYSTICK_PER_SEC           = (SYSCLK_FREQ_24MHz / SYS_TICKS_BETWEEN_SYSTICK_IRQ);

typedef enum
{
    SYS_TICKS_100_MS              = SYS_SYSTICK_PER_SEC / 10,
    SYS_TICKS_250_MS              = SYS_SYSTICK_PER_SEC / 4,
    SYS_TICKS_01_SEC              = 1 * SYS_SYSTICK_PER_SEC,
    SYS_TICKS_10_SEC              = 10 * SYS_SYSTICK_PER_SEC,
} SystemConstant_t;

typedef enum
{
    PIN_LED_BLUE  = GPIO_Pin_8,
    PIN_LED_GREEN = GPIO_Pin_9,
    PIN_BTN_USER  = GPIO_Pin_0,
} PinConfiguration_t;

typedef enum
{
    BTN_PRESSED  = Bit_SET,
    BTN_RELEASED = Bit_RESET,
} ButtonState_t;

typedef enum
{
    UNLOCKED_STATE = 0,
    LOCKED_STATE   = 1
} CSStatus_t;

/** @} */

/**
 * Configures the base peripherals used in this application.
 */
void InitHardware();

/**
 * Reads the state of the user button.
 * @return Returns the state of the user button (i.e. BTN_PRESSED | BTN_RELEASED)
 */
ButtonState_t ReadUserBtn();

/**
 * Turn on an LED.
 * @param led The LED to operate.
 */
void SetLed(PinConfiguration_t led);

/**
 * Turn off an LED.
 * @param led The LED to operate.
 */
void ClrLed(PinConfiguration_t led);

/**
 * Toggle an LED.
 * @param led The LED to operate.
 */
void TglLed(PinConfiguration_t led);

/**
 * Gets the current system tick value.
 *
 * @return Returns the system tick value.
 */
uint32_t GetSysTick();

/**
 * Blocking delay function in milliseconds.
 *
 * @param nTime The number of milliseconds to delay.
 */
void DelayMs(uint32_t nTime);

/**
 * Uses LDREX and STREX instructions to set the lock for a critical section.
 *
 * @param lock - A reference to the lock variable
 * @returns Returns the value of Bsp::LOCKED_STATE if the lock was NOT acquired.
*/
CSStatus_t CSLock(int* lock);

} // namespace Bsp
#endif // #ifndef BSP_H
