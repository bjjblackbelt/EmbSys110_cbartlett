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
#include <stm32f10x_gpio.h>
}

#define SYSTICK_PER_SEC     (SystemCoreClock/App::TICKS_BETWEEN_SYSTICK_IRQ)

namespace App {
/**
 * @defgroup APP_CONSTANTS App Constants
 * @brief Global constants used within this application.
 * @{
 */
//

//!< The number of clock cycles between calls to SysTick_Handler
static const uint32_t TICKS_BETWEEN_SYSTICK_IRQ = 24000;  //!< Hz

typedef enum {
    LED_BLUE     = GPIO_Pin_8,
    LED_GREEN    = GPIO_Pin_9,
    BTN_USER     = GPIO_Pin_0,
} HwIdentifier_t;

typedef enum {
    BTN_PRESSED  = Bit_SET,
    BTN_RELEASED = Bit_RESET,
} ButtonState_t;

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
void SetLed(HwIdentifier_t led);

/**
 * Turn off an LED.
 * @param led The LED to operate.
 */
void ClrLed(HwIdentifier_t led);

/**
 * Toggle an LED.
 * @param led The LED to operate.
 */
void TglLed(HwIdentifier_t led);

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
 * Blocking delay function in milliseconds.
 *
 * @param nTime The number of milliseconds to delay.
 */
void DelayMs(uint32_t nTime);

} // namespace App
#endif // #ifndef BSP_H
