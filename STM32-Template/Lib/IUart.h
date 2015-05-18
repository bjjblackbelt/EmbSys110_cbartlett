/**
 *******************************************************************************
 *  @file   IUart.h
 *  @brief  Declares an interface for a UART peripheral.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#ifndef DUARTINFC_H
#define DUARTINFC_H

#include <stdint.h>

/**
 * @class IUart
 * @brief An interface for a UART peripheral.
 */
class IUart
{
  public:
    /**
     * Constructor
     */
    IUart();

    /**
     * Destructor
     */
    virtual ~IUart();

    /**
     * Initialize the Uart peripheral.
     */
    virtual void Init() = 0;

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
     * Print an unsigned integer in decimal formal.
     * @param dec The number to be printed.
     */
    void PrintUInt(uint32_t dec);

  protected:
    /**
     * Uart put char function
     * @param  c Character to be printed.
     */
    virtual void PutC(uint8_t c) = 0;

    /**
     * Reads a character from the Uart RX line.
     * @return Character read.
     */
    virtual uint8_t GetC() = 0;

  private:
    IUart(const IUart&);            //!< Intentionally not implemented
    IUart& operator=(const IUart&); //!< Intentionally not implemented

    /**
     * Gets the position of the first non-zero nibble.
     *
     * The most significant nibble is in position 0. For example, given a number
     * 0x0000CODE, this function will return 4 (i.e. '0xC' is in position 4).
     *
     * @param number The number to examine.
     * @return The position (indexed from 0) of the first non-zero nibble.
     */
    uint8_t GetPosOfFirstNonZeroNibble(uint32_t number);

    /**
     * Convert nibble to ascii character and print.
     * @param  nibble  The number to be converted.
     */
    void PrintNibble(uint8_t nibble);
};

#endif // #ifndef DUARTINFC_H
