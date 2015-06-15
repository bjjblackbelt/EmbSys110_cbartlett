/**
 *******************************************************************************
 *  @file   IUart.cpp
 *  @brief  Implements the UART interface.
 *  @author Chad Bartlett   <cbartlet@uw.edu>
 *  @date   20150507
 *******************************************************************************
 */
#include "IUart.h"
#include <stddef.h>

static const uint8_t NIBBLES_IN_WORD = 8;
static const uint8_t BITS_IN_NIBBLE  = 4;
#define LEADING_NIBBLE_MASK(nibble)         (0xF << (28 - (BITS_IN_NIBBLE*(nibble))))
#define SHIFT_TO_FIRST_NIBBLE_POS(nibble)   (28 - (BITS_IN_NIBBLE*(nibble)))

#define PRINT_DISABLE_INTERRUPTS (0U)
#if PRINT_DISABLE_INTERRUPTS
#define DISABLE_INTS()      do{__asm volatile ("cpsid i");}while(0)
#define ENABLE_INTS()      do{__asm volatile ("cpsie i");}while(0)
#else
#define DISABLE_INTS()
#define ENABLE_INTS()
#endif

IUart::IUart()
{
}

IUart::~IUart()
{
}

void IUart::PrintStr(char const * const string)
{
    DISABLE_INTS();

    if (string != NULL)
    {
        const char* pString = string;
        while(*pString != '\0')
        {
            PutC((uint8_t)*pString++);
        }
    }

    ENABLE_INTS();
}

void IUart::PrintHex(uint32_t hex)
{
    DISABLE_INTS();

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

    ENABLE_INTS();
}

void IUart::PrintUInt(uint32_t dec)
{
    DISABLE_INTS();

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

    ENABLE_INTS();
}

void IUart::PrintNibble(uint8_t nibble)
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

uint8_t IUart::GetPosOfFirstNonZeroNibble(uint32_t number)
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
