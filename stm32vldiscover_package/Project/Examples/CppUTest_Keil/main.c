#include "stm32f10x.h"
#include <stdint.h>

void Delay(__IO uint32_t nCount);
void LED_Init(void);

#define LED4_ON()		GPIOC->BSRR |= (uint32_t)(1 << 8u)
#define LED4_OFF()	GPIOC->BSRR |= (uint32_t)(1 << 24u)
#define LED5_ON()		GPIOC->BSRR |= (uint32_t)(1 << 9u)
#define LED5_OFF()	GPIOC->BSRR |= (uint32_t)(1 << 25u)

int main(void)
{
	LED_Init();

  while (1)
  {
    /* Turn on LD2 and LD3 */
		LED4_ON();
		LED5_ON();
   /* Insert delay */
    Delay(0xAFFFF);
    /* Turn off LD3 and LD4 */
		LED4_OFF();
		LED5_OFF();
    /* Insert delay */
    Delay(0xAFFFF);

  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void LED_Init()
{
		// Start PortC clocks
	RCC->APB2ENR |= (uint32_t)(1 << 4u);
	// Reset GPIOC pins and set PC8 and PC9 as push-pull output at 50 MHz
	GPIOC->CRH = 0x44444433;
	LED4_OFF();
	LED5_OFF();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
