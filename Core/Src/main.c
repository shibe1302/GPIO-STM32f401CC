#include "main.h"

// Hàm cấu hình clock: dùng HSE = 25MHz, PLL ra SYSCLK = 84MHz
void enable_gpio_clock(void)
{
  // 1. Enable HSE and wait for it to stabilize
  MODIFY_REG(REG32(RCC_CR), 1 << 16, 1 << 16); // Enable HSE

  MODIFY_REG(REG32(FLASH_ACR), 0x7, 0x2);
  // 2. Set AHB, APB1, APB2 prescalers
  MODIFY_REG(REG32(RCC_CFGR), 0xF << 4, 0x0 << 4);   // HPRE Prescaler = 1
  MODIFY_REG(REG32(RCC_CFGR), 0x7 << 10, 0x5 << 10); // APB1 Prescaler = 4 (42MHz)
  MODIFY_REG(REG32(RCC_CFGR), 0x7 << 13, 0x0 << 13); // APB2 Prescaler = 1 (84MHz)

  // 3. Configure PLL
  MODIFY_REG(REG32(RCC_PLLCFGR), 0x3F << 0, 25 << 0);   // PLLM = 25
  MODIFY_REG(REG32(RCC_PLLCFGR), 0x1FF << 6, 336 << 6); // PLLN = 336
  MODIFY_REG(REG32(RCC_PLLCFGR), 0x3 << 16, 1 << 16);   // PLLP = 4 (84MHz)
  MODIFY_REG(REG32(RCC_PLLCFGR), 1 << 22, 1 << 22);     // PLLSRC = HSE

  // 4. Enable PLL and wait for it to stabilize
  MODIFY_REG(REG32(RCC_CR), 1 << 24, 1 << 24); // Enable PLL

  // 5. Switch system clock to HSE
  MODIFY_REG(REG32(RCC_CFGR), 0x3 << 0, 0x2 << 0);

  // 6. Enable GPIOA, GPIOC clock
  MODIFY_REG(REG32(RCC_AHB1ENR), 1 << 2, 1 << 2); // GPIOC
  MODIFY_REG(REG32(RCC_AHB1ENR), 1 << 0, 1 << 0); // GPIOA
}

// Hàm delay đơn giản
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

// Cấu hình GPIO làm output, tốc độ cao
void init_pin(void)
{
  // GPIOC Pin 13 (nếu có LED debug)
  MODIFY_REG(REG32(GPIOC_MODER), (0x03U << (13 * 2)), (GPIO_MODER_OUTPUT << (13 * 2)));
  MODIFY_REG(REG32(GPIOC_OSPEEDR), (0x03U << (13 * 2)), (0x03U << (13 * 2)));

  // GPIOA Pin 0 -> 3
  for (int i = 0; i <= 3; i++)
  {
    MODIFY_REG(REG32(GPIOA_MODER), (0x03U << (i * 2)), (GPIO_MODER_OUTPUT << (i * 2)));
    MODIFY_REG(REG32(GPIOA_OSPEEDR), (0x03U << (i * 2)), (0x03U << (i * 2)));
  }
}

// Bật LED
void led_on(unsigned int pin_num)
{
  SET_BIT(REG32(GPIOA_BSRR), (1 << pin_num));
}

// Tắt LED
void led_off(unsigned int pin_num)
{
  SET_BIT(REG32(GPIOA_BSRR), (1 << (pin_num + 16)));
}

// Hiệu ứng LED chạy đuổi
void led_duoi(void)
{
  for (unsigned int i = 0; i <= 3; i++)
  {
    led_on(i);
    delay1(1000); // Delay tăng lên đủ lâu để dễ nhìn
    led_off(i);
    delay1(1000);
  }
}

int main(void)
{
  enable_gpio_clock(); // Cấu hình clock và bật clock GPIO
  init_pin();          // Cấu hình GPIO output
                       // Bật LED đầu tiên
  while (1)
  {
    led_duoi();
  }
}
