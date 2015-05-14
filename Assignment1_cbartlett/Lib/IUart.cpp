/**
 *******************************************************************************
 *  @file   DUartIF.cpp
 *  @brief  Implements the UART interface.
 *  @author Chad Bartlett   <cbartlet@uw.edu>
 *  @date   20150507
 *******************************************************************************
 */
#include "DUartIF.h"
#include <stddef.h>

static const uint8_t NIBBLES_IN_WORD = 8;
static const uint8_t BITS_IN_NIBBLE  = 4;
#define LEADING_NIBBLE_MASK(nibble)         (0xF << (28 - (BITS_IN_NIBBLE*(nibble))))
#define SHIFT_TO_FIRST_NIBBLE_POS(nibble)   (28 - (BITS_IN_NIBBLE*(nibble)))

DUartIF::DUartIF()
{
}

DUartIF::~DUartIF()
{
}

void DUartIF::PrintStr(char const * const string)
{
    if (string != NULL)
    {
        const char* pString = string;
        while(*pString != '\0')
        {
            PutC((uint8_t)*pString++);
        }
    }
}

void DUartIF::PrintHex(uint32_t hex)
{
    //!< Print the leading characters '0x'
    PrintNibble(static_cast<uint8_t>(0));
    PutC(static_cast<uint8_t>('x'));

    //!< Print the word starting at the first non-zero nibble.
    //!< Always print at least the least significant nibble.
    for (int nibble = GetPosOfFirstNonZeroNibble(hex); nibble < NIBBLES_IN_WORD; ++nibble)
    {
        uint8_t character = static_cast<uint8_t>((hex >> SHIFT_TO_FIRST_NIBBLE_POS(nibble)) & 0xF);
        PrintNibble(character);
    }
}

void DUartIF::PrintUInt(uint32_t dec)
{
    const uint8_t SINGLE_DIGIT_ASCII_CONVERSION = 48;

    uint32_t numberIn = dec;
    uint32_t devisor = 1000000000;
    bool isFirstNonZero = false;
    do
    {
        uint32_t numberOut = numberIn/devisor;
        numberIn -= numberOut*devisor;
        devisor /= 10;

        if (isFirstNonZero == false)
        {
            if (numberOut > 0)
            {
                isFirstNonZero = true;
                PutC(numberOut + SINGLE_DIGIT_ASCII_CONVERSION);
            }
        }
        else
        {
            PutC(numberOut + SINGLE_DIGIT_ASCII_CONVERSION);
        }
    } while (devisor >= 10);

    PutC(numberIn%10 + SINGLE_DIGIT_ASCII_CONVERSION);
}

void DUartIF::PrintNibble(uint8_t nibble)
{
    const uint8_t SINGLE_DIGIT_ASCII_CONVERSION = 48;
    const uint8_t HEX_DIGIT_ASCII_CONVERSION    = 55;

    //!< If the nibble to be converted is in the range 0-9
    if (nibble < 10)
    {
        PutC(nibble + SINGLE_DIGIT_ASCII_CONVERSION);
    }
    else //!< the number is in the range 10-15
    {
        PutC(nibble + HEX_DIGIT_ASCII_CONVERSION);
    }
}

uint8_t DUartIF::GetPosOfFirstNonZeroNibble(uint32_t number)
{
    //!< Determine the number of leading nibbles that are zero
    uint8_t leadingZeroNibbles = 0;
    //!< Start with the most significant nibble and decrement until the nibble
    //!< just before the least significant.
    while (((number & LEADING_NIBBLE_MASK(leadingZeroNibbles)) < 1)
           && (leadingZeroNibbles < (NIBBLES_IN_WORD - 1)))
    {
        ++leadingZeroNibbles;
    }

    return leadingZeroNibbles;
}
