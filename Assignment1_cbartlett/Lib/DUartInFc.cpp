/**
 *******************************************************************************
 *  @file   DUartInFc.cpp
 *  @brief  Implements the UART interface.
 *  @author Chad Bartlett   <cbartlet@uw.edu>
 *  @date   20150507
 *******************************************************************************
 */
#include "DUartInFc.h"
#include <stddef.h>

DUartInFc::DUartInFc()
{
}

DUartInFc::~DUartInFc()
{
}

void DUartInFc::PrintStr(char const * const string)
{
    const char* pString = string;
    while((*pString != '\0') && (pString != NULL))
    {
        PutC((uint8_t)*pString++);
    }
}

void DUartInFc::PrintHex(uint32_t hex)
{
    static const uint8_t NIBBLES_IN_WORD = 8;
    static const uint8_t BITS_IN_NIBBLE  = 4;
    #define LEADING_NIBBLE_MASK(nibble)         (0xF << (28 - (BITS_IN_NIBBLE*(nibble))))
    #define SHIFT_TO_FIRST_NIBBLE_POS(nibble)   (28 - (BITS_IN_NIBBLE*(nibble)))

    //!< Print the leading characters '0x'
    PrintHexCharacter(static_cast<uint8_t>(0));
    PutC(static_cast<uint8_t>('x'));

    //!< Determine the number of leading nibbles that are zero
    uint8_t leadingZeroNibbles = 0;
    //!< Start with the most significant nibble and decrement until the nibble
    //!< just before the least significant.
    while (((hex & LEADING_NIBBLE_MASK(leadingZeroNibbles)) < 1)
           && (leadingZeroNibbles < (NIBBLES_IN_WORD - 1)))
    {
        ++leadingZeroNibbles;
    }

    //!< Print the word starting at the first non-zero nibble.
    //!< Always print at least the least significant nibble.
    for (int nibble = leadingZeroNibbles; nibble < NIBBLES_IN_WORD; ++nibble)
    {
        uint8_t character = static_cast<uint8_t>((hex >> SHIFT_TO_FIRST_NIBBLE_POS(nibble)) & 0xF);
        PrintHexCharacter(character);
    }
}

void DUartInFc::PrintHexCharacter(uint8_t byte)
{
    const uint8_t SINGLE_DIGIT_ASCII_CONVERSION = 48;
    const uint8_t HEX_DIGIT_ASCII_CONVERSION    = 55;

    //!< If the byte to be converted is in the range 0-9
    if (byte < 10)
    {
        PutC(byte + SINGLE_DIGIT_ASCII_CONVERSION);
    }
    else //!< the number is in the range 10-15 so convert to ascii hex A-F
    {
        PutC(byte + HEX_DIGIT_ASCII_CONVERSION);
    }
}
