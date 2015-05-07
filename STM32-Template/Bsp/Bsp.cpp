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
}

namespace App {

void InitHardware()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable Peripheral Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // Configure Pins used for LEDs
    GPIO_StructInit(& GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = LED_GREEN | LED_BLUE;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);

    // Configure user button
    GPIO_StructInit(& GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = BTN_USER;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);

    // Configure SysTick Timer
    if (SysTick_Config(SystemCoreClock / 1000)) while (1);
}

ButtonState_t ReadUserBtn()
{
    return static_cast<ButtonState_t>(GPIO_ReadInputDataBit(GPIOA, BTN_USER));
}

void SetLed(HwIdentifier_t led)
{
    GPIO_WriteBit(GPIOC, led, Bit_SET);
}

void ClrLed(HwIdentifier_t led)
{
    GPIO_WriteBit(GPIOC, led, Bit_RESET);
}

void TglLed(HwIdentifier_t led)
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

} // namespace App


#ifdef USE_FULL_ASSERT
extern "C" void assert_failed(uint8_t* file , uint32_t line)
{
    (void)file; (void)line;
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
# endif
