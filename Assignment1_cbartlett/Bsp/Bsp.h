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

#define TIME_MS_TO_TICK(ms)     (((ms)*App::SYS_SYSTICK_PER_SEC)/1000)

namespace App {
/**
 * @defgroup APP_CONSTANTS App Constants
 * @brief Global constants used within this application.
 * @{
 */
//

//!< The number of clock cycles between calls to SysTick_Handler
typedef enum
{
    SYS_TICKS_BETWEEN_SYSTICK_IRQ = SYSCLK_FREQ_24MHz / 1000, //!< Hz
    SYS_SYSTICK_PER_SEC           = (SYSCLK_FREQ_24MHz / SYS_TICKS_BETWEEN_SYSTICK_IRQ),
    SYS_TICKS_100_MS              = SYS_SYSTICK_PER_SEC / 10,
    SYS_TICKS_250_MS              = SYS_SYSTICK_PER_SEC / 4,
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
 * Resets the value of the system tick counter to zero.
 */
void ResetSysTick();

/**
 * Prints a string.
 * @param string The string to be printed
 */
void PrintStr(char const * const string);

/**
 * Print a number in hexadecimal formal.
 * @param hex The number to be printed.
 */
void PrintHex(uint32_t hex);

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
 * @returns Returns the value of App::LOCKED_STATE if the lock was NOT acquired.
*/
CSStatus_t CSLock(int* lock);

} // namespace App
#endif // #ifndef BSP_H
