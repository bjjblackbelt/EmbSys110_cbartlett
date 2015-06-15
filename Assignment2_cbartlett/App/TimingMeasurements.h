/**
 *******************************************************************************
 *  @file   TimingMeasurements.h
 *  @brief  Definitions for toggling GPIO pins used during timing measurements.
 *  @author Chad Bartlett   <cbartlet@uw.com>
 *******************************************************************************
 */
#ifndef TIMINGMEASUREMENTS_H
#define TIMINGMEASUREMENTS_H

#define ENABLE_TIME_MEAS_SYS_INIT       (0U)
#define ENABLE_TIME_MEAS_CREATE_TASK    (0U)
#define ENABLE_TIME_MEAS_SET_NEXT_TASK  (0U)
#define ENABLE_TIME_MEAS_CONTEXT_SWITCH (0U)
#define ENABLE_TIME_MEAS_TIMETICK       (0U)

//!< PC10
#if ENABLE_TIME_MEAS_SYS_INIT
#define TIME_MEAS_SYS_INIT_START()                  GPIOC->BSRR = GPIO_Pin_10
#define TIME_MEAS_SYS_INIT_STOP()                   GPIOC->BRR = GPIO_Pin_10
#else
#define TIME_MEAS_SYS_INIT_START()
#define TIME_MEAS_SYS_INIT_STOP()
#endif

//!< PC11
#if ENABLE_TIME_MEAS_CREATE_TASK
#define TIME_MEAS_CREATE_TASK_START()               GPIOC->BSRR = GPIO_Pin_11
#define TIME_MEAS_CREATE_TASK_STOP()                GPIOC->BRR = GPIO_Pin_11
#else
#define TIME_MEAS_CREATE_TASK_START()
#define TIME_MEAS_CREATE_TASK_STOP()
#endif

//!< PC12
#if ENABLE_TIME_MEAS_SET_NEXT_TASK
#define TIME_MEAS_CREATE_SET_NEXT_TASK_START()      GPIOC->BSRR = GPIO_Pin_12
#define TIME_MEAS_CREATE_SET_NEXT_TASK_STOP()       GPIOC->BRR = GPIO_Pin_12
#else
#define TIME_MEAS_CREATE_SET_NEXT_TASK_START()
#define TIME_MEAS_CREATE_SET_NEXT_TASK_STOP()
#endif

//!< PC6
#if ENABLE_TIME_MEAS_CONTEXT_SWITCH
#define TIME_MEAS_CONTEXT_SWITCH_START()            GPIOC->BSRR = GPIO_Pin_6
#define TIME_MEAS_CONTEXT_SWITCH_STOP()             GPIOC->BRR = GPIO_Pin_6
#else
#define TIME_MEAS_CONTEXT_SWITCH_START()
#define TIME_MEAS_CONTEXT_SWITCH_STOP()
#endif

//!< PC7
#if ENABLE_TIME_MEAS_TIMETICK
#define TIME_MEAS_TIMETICK_START()                  GPIOC->BSRR = GPIO_Pin_7
#define TIME_MEAS_TIMETICK_STOP()                   GPIOC->BRR = GPIO_Pin_7
#else
#define TIME_MEAS_TIMETICK_START()
#define TIME_MEAS_TIMETICK_STOP()
#endif

//!< If any of the measurements are enabled, enable all timing pins as output
#if  ENABLE_TIME_MEAS_SYS_INIT          || ENABLE_TIME_MEAS_CREATE_TASK  \
     || ENABLE_TIME_MEAS_SET_NEXT_TASK  || ENABLE_TIME_MEAS_CONTEXT_SWITCH \
     || ENABLE_TIME_MEAS_TIMETICK

extern "C" {
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
}

#define CONFIG_TIME_MEAS_GPIOS() \
    do { \
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); \
        GPIO_InitTypeDef GPIO_InitStructure; \
        GPIO_StructInit(& GPIO_InitStructure); \
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; \
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; \
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; \
        GPIO_Init(GPIOC , &GPIO_InitStructure); \
    } while(0)

#else
#define CONFIG_TIME_MEAS_GPIOS()
#endif

#endif // TIMINGMEASUREMENTS_H
