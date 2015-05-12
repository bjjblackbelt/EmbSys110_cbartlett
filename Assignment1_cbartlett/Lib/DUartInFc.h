/**
 *******************************************************************************
 *  @file   DUartInFc.h
 *  @brief  Declares an interface for a UART peripheral.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#ifndef DUARTINFC_H
#define DUARTINFC_H

#include <stdint.h>

/**
 * @class DUartInFc
 * @brief An interface for a UART peripheral.
 */
class DUartInFc
{
  public:
    /**
     * Constructor
     */
    DUartInFc();

    /**
     * Destructor
     */
    virtual ~DUartInFc();

    /**
     * Initialize the Uart peripheral.
     */
    virtual void Init(){;};

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

  protected:
    DUartInFc(const DUartInFc&);            //!< Intentionally not implemented
    DUartInFc& operator=(const DUartInFc&); //!< Intentionally not implemented

    /**
     * Uart put char function
     * @param  c Character to be printed.
     */
    virtual void PutC(uint8_t c){(void)c;}

    /**
     * Reads a character from the Uart RX line.
     * @return Character read.
     */
    virtual uint8_t GetC(){return 0;}

    /**
     * Convert number to ascii character in hexadecimal format and print.
     * @param  byte  The number to be converted.
     */
    void PrintHexCharacter(uint8_t byte);

    /**
     * Convert number to ascii character in decimal format and print.
     * @param  byte  The number to be converted.
     * @return       Returns the number as an ascii character.
     */
    void PrintDecimalCharacter(uint8_t byte);
};

#endif // #ifndef DUARTINFC_H
