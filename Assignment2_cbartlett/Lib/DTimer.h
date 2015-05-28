/**
 *******************************************************************************
 *  @filename   DTimer.h
 *  @brief      See implementation file for class description.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#ifndef DTIMER_H
#define DTIMER_H

#include <stdint.h>

class DTimer
{
  public:
    static const uint32_t TIMER_PRESCALER = 128;
    static const uint32_t TIMER_COMPARE_VALUE = 375;

    /**
     * Constructor
     */
    DTimer();
    /**
     * Destructor
     */
    ~DTimer();

    /**
     * Configures
     */
    void Open();
    void Start();
    void Stop();

    /**
     * Reset the registers of the timer peripheral.
     */
    void Close();

    static void TIM6_DAC_IRQHandler();

  private:
    DTimer(const DTimer&);            //!< Intentionally not implemented
    DTimer& operator=(const DTimer&); //!< Intentionally not implemented
};

#endif // #ifndef DTIMER_H
