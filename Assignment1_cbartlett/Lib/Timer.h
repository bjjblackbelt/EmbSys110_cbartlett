/**
 *******************************************************************************
 *  @filename   Timer.h
 *  @brief      See implementation file for class description.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "lpc2378.h"

/**
 * This macro is used to compute the compare register value for setting the
 * timer period.
 * @param freq The desired frequency of the timer interrupt.
 */
#define TIMER_COMPARE_VALUE(freq) 		(PCLK/((freq)*Timer::TIMER_PRESCALER))

class Timer
{
  public:
    static const uint32_t TIMER_PRESCALER = 4000;

    typedef enum
    {
        TIMER_00 = 0,
        TIMER_01 = 1,
    } TimerNumber_t;

    Timer();
    ~Timer();

    void Open(TimerNumber_t timer, uint32_t timerInterval);
    void Start(TimerNumber_t timer);
    void Stop(TimerNumber_t timer);
    void Close(TimerNumber_t timer);

    static void Timer0IntHandler();
    static void Timer1IntHandler();

  private:
    Timer(const Timer&);            //!< Intentionally not implemented
    Timer& operator=(const Timer&); //!< Intentionally not implemented

    static void ProcessCounterTest(int threadID);

    static Timer_t* const s_timers[];
    static volatile bool s_isErrorState; // TODO: CMB - Remove after testing
};

#endif // #ifndef TIMER_H
