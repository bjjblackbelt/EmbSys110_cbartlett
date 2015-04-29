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
void Timer::Init(TimerNumber_t timer)
{
}
void Timer::Start(TimerNumber_t timer)
{
}
void Timer::Stop(TimerNumber_t timer)
{
}
void Timer::Reset(TimerNumber_t timer)
{
}
