/**
 *******************************************************************************
 *  @file   DUart.cpp
 *  @brief  ements the UART interface.
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
: IUart()
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

    // Modify USART_InitStructure for non -default values,
    //  e.g. USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_BaudRate = USART1_BAUD_RATE;
    USART_InitStructure.USART_Mode     = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1 , &USART_InitStructure);
    USART_Cmd(USART1 , ENABLE);
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
