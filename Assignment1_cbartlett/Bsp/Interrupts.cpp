/**
 *******************************************************************************
 *  @file   Interrupts.cpp
 *  @brief  Implements system interrupts.
 *  @author Chad Bartlett   <cbartlet@uw.edu>
 *  @date   20150507
 *******************************************************************************
 */
extern "C" {
#include <stm32f10x.h>
}

#include <stdint.h>
#include <stm32f10x_conf.h>
#include "Bsp.h"
#include "OS.h"

namespace Bsp {
volatile uint32_t g_sysTick = 0;
extern OS* g_pOS;
}


/*..........................................................................*/
extern "C" void Reset_Handler(void)
{
    extern int main(void);
    extern int __libc_init_array(void);
    extern unsigned __data_start;    /* start of .data in the linker script */
    extern unsigned __data_end__;      /* end of .data in the linker script */
    extern unsigned const __data_load;  /* initialization values for .data  */
    extern unsigned __bss_start__;    /* start of .bss in the linker script */
    extern unsigned __bss_end__;        /* end of .bss in the linker script */
    unsigned const *src;
    unsigned *dst;

    /* copy the data segment initializers from flash to RAM... */
    src = &__data_load;
    for (dst = &__data_start; dst < &__data_end__; ++dst, ++src)
    {
        *dst = *src;
    }

    /* zero fill the .bss segment... */
    for (dst = &__bss_start__; dst < &__bss_end__; ++dst)
    {
        *dst = 0;
    }

    /* Use the STM32 Libraries to initialize system */
    SystemInit();
    Bsp::ResetSysTick();

    /* call all static constructors in C++ (harmless in C programs) */
    __libc_init_array();

    /* call the application's entry point */
    main();

    /* in a bare-metal system main() has nothing to return to and it should
    * never return. Just in case main() returns, the assert_failed() gives
    * the last opportunity to catch this problem.
    */
    assert_param(false);
}

extern "C" void SysTick_Handler(void)
{
    static volatile uint32_t nextLedToggle = Bsp::SYS_TICKS_100_MS;

    if (nextLedToggle == Bsp::g_sysTick++)
    {
        Bsp::TglLed(Bsp::PIN_LED_BLUE);
        nextLedToggle += Bsp::SYS_TICKS_100_MS;
    }

    Bsp::g_pOS->Scheduler();
}


