/**
 *******************************************************************************
 *  @file   Bsp.cpp
 *  @brief  Implements board specific functions
 *  @author Chad Bartlett   <cbartlet@uw.edu>
 *  @date   20150507
 *******************************************************************************
 */
#include <stdint.h>
#include <stddef.h>
#include "Bsp.h"
#include "DUart.h"

extern "C" {
#include <stm32f10x_rcc.h>
}

// Prototypes
/** Initialize the board LEDs */
static void Led_Init();
/** Initialize the USER button */
static void Button_Init();

static DUart* s_pUart = NULL;

namespace App {

void InitHardware(DUart& uart)
{
    s_pUart = &uart;

    Led_Init();
    Button_Init();
    s_pUart->Init();

    // Configure SysTick Timer
    if (SysTick_Config(TICKS_BETWEEN_SYSTICK_IRQ)) while (1);
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
    uint32_t nTicks = TIME_MS_TO_TICK(nTime);
    ResetSysTick();
    while (GetSysTick() < nTicks);
}

void PrintStr(char const * const string)
{
    s_pUart->PrintStr(string);
}

void PrintHex(uint32_t hex)
{
    s_pUart->PrintHex(hex);
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

#ifdef USE_FULL_ASSERT
extern "C" void assert_failed(uint8_t* file , uint32_t line)
{
    (void)file; (void)line;
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
# endif

