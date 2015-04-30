/*
 *******************************************************************************
 * @file main.cpp
 * @brief An application to test this context switch implementation.
 *
 * Two timers are configured to trigger interrupts at 100-ms and 500-ms. During
 * normal operation an LED is toggled within the 100-ms timer interrupt. In
 * addition, within both interrupts handlers an attempt is made to take a
 * critical section. If successful, a shared variable is incremented, checked
 * for the correct value and subsequently decremented. If the check fails the
 * system enters an error state for 5-seconds. During the error state, the LED
 * is steady on. At the completion of the error state the shared variable is
 * reset and the LED commences toggling once again.
 *
 *******************************************************************************
 */
#define __MAIN_C__

#include <stdint.h>
#include <stddef.h>
#include "lpc2378.h"
#include "print.h"
#include "bsp.h"
#include "Timer.h"

/**
 * @typedef enum USER
 * The buttons and joystick are polled during a GPIO read. These values indicate
 * the type of input event.
 */
typedef enum
{
    USER_NONE   = 0,
    USER_BUT1   = 1,
    USER_BUT2   = 2,
    USER_CENTER = 3,
    USER_UP     = 4,
    USER_DOWN   = 5,
    USER_LEFT   = 6,
    USER_RIGHT  = 7,
    TOTAL_BTNS  = 8,
} USER;

static const int TIMER_PERIOD_500_MS = 5000;
static const int TIMER_PERIOD_100_MS = 1000;

/**
 * Strings to indicate the type of input event
 */
const char* s_msgTemplate = { "\nButton pressed: " };
const char* s_id[TOTAL_BTNS] = { "NONE", "BUT1", "BUT2", "CENTER", "UP", "DOWN", "LEFT", "RIGHT" };

// Prototypes
extern "C" {
    extern int gpio_read(void);
    extern void led_set(void);
    extern void led_clr(void);
    extern void delayMS(uint32_t ms);
}

/**
 * Start of the context switch test application.
 * @return  Does not return
 */
int main(void)
{
    /* initialize */
    BSP_InitHardware();

    // TODO: CMB - Test LED functionality and timer register memory layout
    led_clr();
    led_set();
    led_clr();
    volatile uint16_t uartBaud = UART_BAUD(HOST_BAUD_U0);
    (void)uartBaud;

    //TODO: CMB - Remove
    int temp = 0;
    BSP_CSLock(temp);

    // Initialize and start timers
    Timer timer;
    timer.Open(Timer::TIMER_00, TIMER_PERIOD_500_MS);
    timer.Open(Timer::TIMER_01, TIMER_PERIOD_100_MS);
    timer.Start(Timer::TIMER_00);
    timer.Start(Timer::TIMER_01);

    volatile Timer_t* timer0 = TIMER0;
    volatile Timer_t* timer1 = TIMER1;
    (void)timer0; (void)timer1;

    printString("\033[2J"); /* Clear entire screen */
    printString("\nChad Bartlett's HW 01: Context Switch Using SWP");

    while (1)
    {
        int button = gpio_read();
        if ((button != USER_NONE) && (button < TOTAL_BTNS))
        {
            printString(s_msgTemplate);
            printString(s_id[button]);
        }

        delayMS(100);
    }

    return (0);
}
