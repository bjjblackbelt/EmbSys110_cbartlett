/**
 *******************************************************************************
 *  @filename   CriticalSection.cpp
 *  @brief      Implements a low-level driver for the timer peripheral.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "Timer.h"

Timer_t* const s_timers[] = {TIMER0, TIMER1};

/**
 * Initialize the timer peripheral.
 * @param timer The timer to initialize
 */
void Timer::Open(TimerNumber_t timer, uint32_t timerInterval)
{
    // Enable Power bits
    switch (timer)
    {
        case TIMER_00:
        {
            // Enable power
            PCONP |= BIT(1);

            // Enable peripheral clock
            PCLKSEL0 &= ~(0x00000004);
            PCLKSEL0 |= BIT(2);
            break;
        }
        case TIMER_01:
        {
            // Enable power
            PCONP |= BIT(2);

            // Enable peripheral clock
            PCLKSEL0 &= ~(0x00000030);
            PCLKSEL0 |= BIT(4);
            break;
        }
    }

    // Configure the timer
    s_timers[timer]->CTCR |= 0x00000000; /* Timer Mode: PC incremented every rising PCLK edge */
    s_timers[timer]->PR   = 200;
    s_timers[timer]->MR0  = timerInterval;
    s_timers[timer]->MCR  |= 0x00000003; /* Interrupt & reset on MR0: an interrupt is generated when MR0
                                            matches the value in the TC and TC is then reset. */
}

void Timer::Start(TimerNumber_t timer)
{
    // Enable counter
    s_timers[timer]->TCR  |= 0x00000001;

    // Enable interrupt
}
void Timer::Stop(TimerNumber_t timer)
{
    // Disable counter
    s_timers[timer]->TCR  &= ~(0x00000001);

    // Disable interrupt
}

/**
 * Reset the registers of the timer peripheral.
 * @param timer The timer to reset
 */
void Timer::Close(TimerNumber_t timer)
{
    // Disable Power bits
    switch (timer)
    {
        case TIMER_00:
        {
            PCONP &= ~(BIT(1));
            PCLKSEL0 &= ~(0x00000004);
            break;
        }
        case TIMER_01:
        {
            PCONP &= ~(BIT(2));
            PCLKSEL0 &= ~(0x00000030);
            break;
        }
    }

    s_timers[timer]->IR   &= ~(0x0000003F);
    s_timers[timer]->TCR  &= ~(0x00000002);
    s_timers[timer]->TC   = 0x00000000;
    s_timers[timer]->PC   = 0x00000000;
    s_timers[timer]->MCR  &= ~(0x00000FFF);
    s_timers[timer]->MR0  = 0x00000000;
    s_timers[timer]->MR1  = 0x00000000;
    s_timers[timer]->MR2  = 0x00000000;
    s_timers[timer]->MR3  = 0x00000000;
    s_timers[timer]->CCR  &= ~(0x0000003F);
    s_timers[timer]->EMR  &= ~(0x00000FFF);
    s_timers[timer]->CTCR &= ~(0x0000000F);
}
