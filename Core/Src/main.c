
#include "main.h"

void enable_gpio_clock(void)
{

  MODIFY_REG(*((unsigned long int *)(RCC_AHB1ENR)), 1 << 2, 1 << 2); // Enable GPIOC clock
}
void delay1(unsigned int timeout)
{
  unsigned int t1, t2;
  for (t1 = 0; t1 < timeout; t1++)
  {
    for (t2 = 0; t2 < 0xFFF; t2++)
    {
      asm("nop");
    }
  }
}

void init_pin(void)
{
  MODIFY_REG(*((unsigned long int *)(GPIOC_MODER)), (0x03U << (13 * 2)), (GPIO_MODER_OUTPUT << (13 * 2)));
}
void led_on(unsigned int pin_num)
{
  SET_BIT(*(unsigned long int *)GPIOC_BSRR, (1 << pin_num)); // Set pin 0 high
}

void led_off(unsigned int pin_num)
{
  SET_BIT(*(unsigned long int *)GPIOC_BSRR, (1 << (pin_num + 16)));
}

int main(void)
{
  enable_gpio_clock();
  init_pin();
  while (1)
  {
    led_on(13);
    delay1(600);
    led_off(13);
    delay1(600);
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
