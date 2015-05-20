/**
 *******************************************************************************
 *  @filename   DTimer.cpp
 *  @brief      Implements a low-level driver for the timer peripheral.
 *  @author     Chad Bartlett   <cbartlet@uw.edu>
 *******************************************************************************
 */
#include "DTimer.h"
#include "Bsp.h"
#include <stddef.h>
#include <stm32f10x.h>

extern "C" {
#include <misc.h>
}

DTimer::DTimer()
{
	//!< Empty
}

DTimer::~DTimer()
{
	//!< Empty
}

void DTimer::Open()
{
	// Enable peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	// Configure the timer
    TIM6->CR1   |= static_cast<uint16_t>(TIM_CR1_ARPE)  /* TIMx_ARR register is buffered. */
                |  static_cast<uint16_t>(TIM_CR1_URS);  /* Only counter overflow/underflow generates an update interrupt. */

    TIM6->PSC = DTimer::TIMER_PRESCALER;
    TIM6->ARR = DTimer::TIMER_COMPARE_VALUE;
}

void DTimer::Start()
{
    // Enable the global timer interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Enable counter
    TIM6->CR1   |=  static_cast<uint16_t>(TIM_CR1_CEN);

    // Enable timer interrupt event
    TIM6->DIER  |=  static_cast<uint16_t>(TIM_DIER_UIE);
}

void DTimer::Stop()
{
    // Disable timer interrupt event
    TIM6->DIER  &=  static_cast<uint16_t>(~TIM_DIER_UIE);

	// Disable counter
    TIM6->CR1   &=  static_cast<uint16_t>(~TIM_CR1_CEN);

    // Disable the global timer interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void DTimer::Close()
{
	// Disable
    RCC->APB1ENR &= static_cast<uint16_t>(~RCC_APB1ENR_TIM6EN)
                 &   static_cast<uint16_t>(~TIM_CR1_URS);

	// Reset timer registers
    TIM6->CR1   &= static_cast<uint16_t>(~TIM_CR1_ARPE);

    TIM6->PSC = 0;
    TIM6->ARR = 0;
}

/**
 * DTimer 0 interrupt handler
 */
extern "C" void DTimer::TIM6_DAC_IRQHandler()
{
    if (TIM6->SR & TIM_SR_UIF)
    {
        TIM6->SR &= static_cast<uint16_t>(~TIM_SR_UIF);
        Bsp::TglLed(Bsp::PIN_LED_GREEN);
    }

}


