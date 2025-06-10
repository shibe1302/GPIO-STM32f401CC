/**
 * @file    system_stm32f4xx.c
 * @brief   Simple System Init for STM32F401 (bare-metal, no HAL)
 */

// Include your custom header file here
#include "main.h" // Thay đổi tên file header cho phù hợp

/**
 * @brief  System Clock Configuration
 *         Hàm này được gọi từ startup file
 *         Bạn có thể để trống hoặc cấu hình clock cơ bản
 * @param  None
 * @retval None
 */
void SystemInit(void)
{
  // Hàm đơn giản để tránh lỗi linker
  // Bạn có thể thêm cấu hình clock cơ bản ở đây nếu cần

  // Ví dụ: Enable HSI (16MHz internal oscillator) - mặc định
  // Không cần làm gì vì HSI đã được enable từ startup
}

/**
 * @brief  Simple delay function
 * @param  count: delay count
 * @retval None
 */
void delay(volatile uint32_t count)
{
  while (count--)
    ;
}