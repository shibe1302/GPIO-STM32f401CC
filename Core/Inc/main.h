
#ifndef __MAIN_H
#define __MAIN_H

/* USER CODE BEGIN EM */
#include <stdint.h>

/* clock */
#define BASE_RCC 0x40023800U
#define RCC_AHB1ENR (BASE_RCC + 0x30U)

/* USER CODE END EM */
#define BASE_GPIOA 0x40020000U
#define BASE_GPIOC 0x40020800U
#define GPIOA_MODER (BASE_GPIOA + 0x00U)
#define GPIOC_MODER (BASE_GPIOC + 0x00U)
#define GPIO_MODER_INPUT 0x00U
#define GPIO_MODER_OUTPUT 0x01U
#define GPIO_MODER_ALTERNATE 0x02U
#define GPIO_MODER_ANALOG 0x03U

#define GPIOC_BSRR (BASE_GPIOC + 0x18U) // GPIO port set/reset register

#define SET_BIT(REG, BIT) ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT) ((REG) & (BIT))

#define CLEAR_REG(REG) ((REG) = (0x0))

#define WRITE_REG(REG, VAL) ((REG) = (VAL))

#define READ_REG(REG) ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/* led */

// Assuming LED is connected to pin 0x01

/* funtion for with REGISTER */

#endif /* __MAIN_H */
