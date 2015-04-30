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

class Timer
{
  public:
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

  private:
    Timer(const Timer&);            //!< Intentionally not implemented
    Timer& operator=(const Timer&); //!< Intentionally not implemented

    static Timer_t* const s_timers[];
};

#endif // #ifndef TIMER_H
