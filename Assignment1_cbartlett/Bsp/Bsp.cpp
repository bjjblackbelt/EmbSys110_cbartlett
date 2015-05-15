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
#include "IUart.h"

extern "C" {
#include <stm32f10x_rcc.h>
}

// Global objects
namespace Bsp {
extern IUart* g_pUart;
}

// Prototypes
/** Initialize the board LEDs */
static void Led_Init();
/** Initialize the USER button */
static void Button_Init();

namespace Bsp {

void InitHardware()
{
    Led_Init();
    Button_Init();

    // Configure SysTick Timer
    if (SysTick_Config(SYS_TICKS_BETWEEN_SYSTICK_IRQ)) while (1);
}

ButtonState_t ReadUserBtn()
{
    return static_cast<ButtonState_t>(GPIO_ReadInputDataBit(GPIOA, PIN_BTN_USER));
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
    uint32_t nTicks = GetSysTick() + TIME_MS_TO_TICK(nTime);
    while (GetSysTick() < nTicks);
}

CSStatus_t CSLock(int* lock)
{
	CSStatus_t lockStatus;

	asm volatile ("MOV      r1, %[lockedState];"                               /* Load the 'lock taken' value */
                  "LDREX    r0, [%[lockAddr]];"                                /* Load the lock value */
                  "STREX    r0, r1, [%[lockAddr]];"                            /* r0 = lock -> lock = LOCKED_STATE */
                  "MOV      %[lockStatus], r0;"                                /* lockedStatus = r0 */
			      : [lockAddr] "=r" (lock), [lockStatus] "=r" (lockStatus)     /* Outputs: %0, %1 */
				  : "[lockAddr]" (lock), [lockedState] "I" (LOCKED_STATE) /* Inputs:  %2, %3 */
                  : "r0", "r1"                                                 /* Clobbered Regs  */
				  );

    return lockStatus;
}

} // namespace Bsp


static void Led_Init()
{
    // Enable Peripheral Clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // Configure Pins used for LEDs
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(& GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = Bsp::PIN_LED_GREEN | Bsp::PIN_LED_BLUE;
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
    GPIO_InitStructure.GPIO_Pin   = Bsp::PIN_BTN_USER;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_InitStructure);
}

#ifdef USE_FULL_ASSERT
extern "C" void assert_failed(uint8_t* file , uint32_t line)
{
    if (Bsp::g_pUart != NULL)
    {
        Bsp::g_pUart->PrintStr("ASSERT: ");
        Bsp::g_pUart->PrintStr((char const * const)file);
        Bsp::g_pUart->PrintStr(" Line: ");
        Bsp::g_pUart->PrintHex(line);
        Bsp::g_pUart->PrintStr("\n");
    }

    Bsp::SetLed(Bsp::PIN_LED_BLUE);
    Bsp::SetLed(Bsp::PIN_LED_GREEN);

    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
# endif

