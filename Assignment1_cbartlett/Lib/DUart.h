/**
 *******************************************************************************
 *  @file   DUart.h
 *  @brief  Declares an interface for a UART peripheral.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#ifndef DUART_H
#define DUART_H

#include "DUartInFc.h"
#include <stdint.h>

/**
 * @class DUart
 * @brief An interface for a UART peripheral.
 * @see DUartInFc.h
 */
class DUart : public DUartInFc
{
  public:
    /**
     * Constructor
     */
    DUart();

    /**
     * Destructor
     */
    virtual ~DUart();

    /**
     * Initialize the Uart peripheral.
     */
    virtual void Init();

  protected:
    DUart(const DUart&);            //!< Intentionally not implemented
    DUart& operator=(const DUart&); //!< Intentionally not implemented

    /**
     * Uart put char function
     * @param  c Character to be printed.
     */
    virtual void PutC(uint8_t c);

    /**
     * Reads a character from the Uart RX line.
     * @return Character read.
     */
    virtual uint8_t GetC();
};

#endif // #ifndef DUART_H
