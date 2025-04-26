/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "uart.h"
#include "work.h"
#include "support.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led1_Pin GPIO_PIN_13
#define led1_GPIO_Port GPIOC
#define led2_Pin GPIO_PIN_14
#define led2_GPIO_Port GPIOC
#define keyStart_Pin GPIO_PIN_3
#define keyStart_GPIO_Port GPIOA
#define out0_Pin GPIO_PIN_0
#define out0_GPIO_Port GPIOB
#define out1_Pin GPIO_PIN_1
#define out1_GPIO_Port GPIOB
#define out2_Pin GPIO_PIN_2
#define out2_GPIO_Port GPIOB
#define led_Pin GPIO_PIN_8
#define led_GPIO_Port GPIOA
#define voltage5_Pin GPIO_PIN_10
#define voltage5_GPIO_Port GPIOA
#define voltage3_3_Pin GPIO_PIN_11
#define voltage3_3_GPIO_Port GPIOA
#define out3_Pin GPIO_PIN_3
#define out3_GPIO_Port GPIOB
#define out4_Pin GPIO_PIN_4
#define out4_GPIO_Port GPIOB
#define out5_Pin GPIO_PIN_5
#define out5_GPIO_Port GPIOB
#define out6_Pin GPIO_PIN_6
#define out6_GPIO_Port GPIOB
#define out7_Pin GPIO_PIN_7
#define out7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
