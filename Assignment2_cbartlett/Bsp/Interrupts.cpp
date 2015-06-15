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
#include <core_cm3.h>
#include "Bsp.h"
#include "OS.h"
#include "TimingMeasurements.h"

namespace Bsp {
volatile uint32_t g_sysTick = 0;
}

namespace OS {
extern State_t g_os;
}

extern "C" {
    void Reset_Handler() __attribute__((isr));
    void SysTick_Handler() __attribute__((isr));
    void PendSV_Handler()  __attribute__((isr, naked));
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

    // Configure pins for timing measurements
    CONFIG_TIME_MEAS_GPIOS();

    /* Use the STM32 Libraries to initialize system */
    TIME_MEAS_SYS_INIT_START();
    SystemInit();

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
    Bsp::g_sysTick++;

    TIME_MEAS_TIMETICK_START();
    OS::TimeTick();
    TIME_MEAS_TIMETICK_STOP();

    TIME_MEAS_CREATE_SET_NEXT_TASK_START();
    OS::SetNextReadyThread();
    TIME_MEAS_CREATE_SET_NEXT_TASK_STOP();

    if (OS::g_os.currThread != OS::g_os.nextThread)
    {
        // Trigger PendSV exception
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
}

extern "C" void PendSV_Handler(void)
{
    // save context
    asm volatile(
        "mrs    r0, psp;"
        "stmdb  r0!, {r4 - r11};"
    );

    TIME_MEAS_CONTEXT_SWITCH_START();

    // Update the current task's stack pointer
    register uint32_t psp asm("r0");
    OS::g_os.threadQueue[OS::g_os.currThread]->stack = (uint32_t*)psp;

    // Get PSP value from next task
    psp = (uint32_t)OS::g_os.threadQueue[OS::g_os.nextThread]->stack;

    // Restore a previously ready state
    if (OS::g_os.threadQueue[OS::g_os.currThread]->state == Thread::STATE_ACTIVE)
    {
        OS::g_os.threadQueue[OS::g_os.currThread]->state = Thread::STATE_READY;
    }

    // Set current thread id after context switch
    OS::g_os.currThread = OS::g_os.nextThread;

    TIME_MEAS_CONTEXT_SWITCH_STOP();

    // Restore the context
    asm volatile(
        "ldmia  r0!, {r4 - r11};"
        "msr    psp, r0;"
        "bx     lr;"
    );
}
