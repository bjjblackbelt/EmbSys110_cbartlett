/**
 *******************************************************************************
 *  @file   Bsp.cpp
 *  @brief  Implements board specific functions
 *  @author Chad Bartlett   <cbartlet@uw.edu>
 *  @date   20150507
 *******************************************************************************
 */
#include <stdint.h>
#include "Bsp.h"

extern "C" {
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>
}

// Prototypes
/** Initialize the board LEDs */
static void Led_Init();
/** Initialize the USER button */
static void Button_Init();
/** Initialize USART1 */
static void Uart_Init();
/** Uart put char function */
static uint8_t Uart_PutC(uint8_t c);
/** Uart get char function */
static uint8_t Uart_GetC();

namespace App {

void InitHardware()
{
    Led_Init();
    Button_Init();
    Uart_Init();

    // Configure SysTick Timer
    if (SysTick_Config(SystemCoreClock / 1000)) while (1);
}

ButtonState_t ReadUserBtn()
{
    return static_cast<ButtonState_t>(GPIO_ReadInputDataBit(GPIOA, App::PIN_BTN_USER));
}

void SetLed(PinConfiguration_t led)
{
    GPIO_WriteBit(GPIOC, led, Bit_SET);
}

void ClrLed(PinConfiguration_t led)
{
    GPIO_WriteBit(GPIOC, led, Bit_RESET);
}

void TglLed(PinConfiguration_t led)
{
    uint8_t bitStatus = GPIO_ReadOutputDataBit(GPIOC, led);

    switch (bitStatus)
    {
        case Bit_RESET:
        {
            GPIO_WriteBit(GPIOC, led, Bit_SET);
            break;
        }
        case Bit_SET:
        {
            GPIO_WriteBit(GPIOC, led, Bit_RESET);
            break;
        }
        default:
        {
            //!< Do nothing
            break;
        }
    }
}

extern volatile uint32_t g_sysTick;

uint32_t GetSysTick()
{
    return g_sysTick;
}

void ResetSysTick()
{
    g_sysTick = 0;
}

void DelayMs(uint32_t nTime)
{
    ResetSysTick();
    while (GetSysTick() < nTime);
}

void PrintStr(char const * const string)
{
    const char* pString = string;
    while(*pString != '\0')
    {
        Uart_PutC((uint8_t)*pString++);
    }
}

uint8_t ConvertHexToAscii(uint8_t);

void PrintHex(uint32_t hex)
{
    const uint8_t NIBBLES_IN_WORD = 8;
    const uint8_t BITS_IN_NIBBLE  = 4;
    #define LEADING_NIBBLE_MASK(nibble)         (0xF << (28 - (BITS_IN_NIBBLE*(nibble))))
    #define SHIFT_TO_FIRST_NIBBLE_POS(nibble)   (28 - (BITS_IN_NIBBLE*(nibble)))

    Uart_PutC(ConvertHexToAscii(static_cast<uint8_t>(0)));
    Uart_PutC(static_cast<uint8_t>('x'));

    uint8_t leadingZeroNibbles = 0;
    while (((hex & LEADING_NIBBLE_MASK(leadingZeroNibbles)) < 1)
           && (leadingZeroNibbles < (NIBBLES_IN_WORD - 1)))
    {
        ++leadingZeroNibbles;
    }

    for (int nibble = leadingZeroNibbles; nibble < NIBBLES_IN_WORD; ++nibble)
    {
        uint8_t character = static_cast<uint8_t>((hex >> SHIFT_TO_FIRST_NIBBLE_POS(nibble)) & 0xF);
        Uart_PutC(ConvertHexToAscii(character));
    }
}

uint8_t ConvertHexToAscii(uint8_t byte)
{
    const uint8_t SINGLE_DIGIT_ASCII_CONVERSION = 48;
    const uint8_t HEX_DIGIT_ASCII_CONVERSION    = 55;
    if (byte < 10)
    {
        return (byte + SINGLE_DIGIT_ASCII_CONVERSION);
    }
    else
    {
        return (byte + HEX_DIGIT_ASCII_CONVERSION);
    }
}

} // namespace App


static void Led_Init()
{
    // Enable Peripheral Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // Configure Pins used for LEDs
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(& GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = App::PIN_LED_GREEN | App::PIN_LED_BLUE;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);
}

static void Button_Init()
{
    // Enable Peripheral Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Configure user button
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(& GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = App::PIN_BTN_USER;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
}

static void Uart_Init()
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
    GPIO_InitStructure.GPIO_Pin   = App::PIN_UART_TX;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Initialize USART1_RX
    GPIO_StructInit(& GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = App::PIN_UART_RX;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Initialize USART structure
    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(& USART_InitStructure);

    // Modify USART_InitStructure for non -default values , e.g.
    // USART_InitStructure.USART_BaudRate = 38400;

    USART_InitStructure.USART_BaudRate = App::USART1_BAUD_RATE;
    USART_InitStructure.USART_Mode     = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1 , & USART_InitStructure);
    USART_Cmd(USART1 , ENABLE);
}

uint8_t Uart_PutC(uint8_t c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART1->DR = (c & 0xff);
    return 0;
}

uint8_t Uart_GetC()
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return  USART1->DR & 0xff;
}

#ifdef USE_FULL_ASSERT
extern "C" void assert_failed(uint8_t* file , uint32_t line)
{
    (void)file; (void)line;
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
# endif

