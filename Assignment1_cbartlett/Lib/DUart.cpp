/**
 *******************************************************************************
 *  @file   DUart.cpp
 *  @brief  Implements the UART interface.
 *  @author Chad Bartlett   <cbartlet@uw.edu>
 *  @date   20150507
 *******************************************************************************
 */
#include "DUart.h"
#include <stddef.h>

extern "C" {
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
}

static const uint16_t USART1_RX_PIN    = GPIO_Pin_10;
static const uint16_t USART1_TX_PIN    = GPIO_Pin_9;
static const uint32_t USART1_BAUD_RATE = 115200;

DUart::DUart()
{
}

DUart::~DUart()
{
}

void DUart::Init()
{
    // Enable Peripheral Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | /* Uart pins */
                           RCC_APB2Periph_AFIO  | /* Alternate function */
                           RCC_APB2Periph_USART1, /* USART1 */
                           ENABLE);

    // Configure USART1
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(& GPIO_InitStructure);

    // Initialize USART1_TX
    GPIO_InitStructure.GPIO_Pin   = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Initialize USART1_RX
    GPIO_StructInit(& GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = USART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Initialize USART structure
    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(& USART_InitStructure);

    // Modify USART_InitStructure for non -default values , e.g.
    //  USART_InitStructure.USART_BaudRate = 38400;

    USART_InitStructure.USART_BaudRate = USART1_BAUD_RATE;
    USART_InitStructure.USART_Mode     = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1 , &USART_InitStructure);
    USART_Cmd(USART1 , ENABLE);
}

void DUart::PrintStr(char const * const string)
{
    const char* pString = string;
    while((*pString != '\0') && (pString != NULL))
    {
        PutC((uint8_t)*pString++);
    }
}

void DUart::PrintHex(uint32_t hex)
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

void DUart::PutC(uint8_t c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART1->DR = (c & 0xff);
}

uint8_t DUart::GetC()
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return  USART1->DR & 0xff;
}

void DUart::PrintHexCharacter(uint8_t byte)
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
