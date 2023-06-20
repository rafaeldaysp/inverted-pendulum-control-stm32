/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PENDULUM_ENCODER_A_Pin GPIO_PIN_0
#define PENDULUM_ENCODER_A_GPIO_Port GPIOA
#define PENDULUM_ENCODER_B_Pin GPIO_PIN_1
#define PENDULUM_ENCODER_B_GPIO_Port GPIOA
#define MOTOR_ENCODER_A_Pin GPIO_PIN_6
#define MOTOR_ENCODER_A_GPIO_Port GPIOA
#define MOTOR_ENCODER_B_Pin GPIO_PIN_7
#define MOTOR_ENCODER_B_GPIO_Port GPIOA
#define MOTOR_PWM_A_Pin GPIO_PIN_8
#define MOTOR_PWM_A_GPIO_Port GPIOA
#define MOTOR_PWM_B_Pin GPIO_PIN_9
#define MOTOR_PWM_B_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
