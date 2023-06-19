/*
 * hw.c
 *
 *  Created on: Jun 13, 2023
 *      Author: Rafael
 */

#include "hw.h"
#include "main.h"
#include "app.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;

void hw_pwm_start(uint8_t timer_number, uint8_t channel){
	if (timer_number == 1){
		if (channel == 1) HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		if (channel == 2) HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	}
}
void hw_encoder_start(uint8_t timer_number){
	if (timer_number == 2) HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
	if (timer_number == 3) HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
}

uint32_t hw_get_encoder_count(uint8_t encoder_number){
	if(encoder_number == 2) return __HAL_TIM_GET_COUNTER(&htim2);
	if(encoder_number == 3) return __HAL_TIM_GET_COUNTER(&htim3);
	return 0;
}

void hw_uart_receive_dma(uint8_t uart_number, uint8_t *buffer, uint16_t size){
	if (uart_number == 1) HAL_UART_Receive_DMA(&huart1, buffer, size);
}

void hw_uart_transmit_it(uint8_t uart_number, uint8_t *buffer, uint16_t size_data){
	if(uart_number == 1) HAL_UART_Transmit_IT(&huart1, buffer, size_data);
}


void hw_set_pwm_dutycycle(uint8_t timer_number, uint32_t channel, uint32_t Dutycyle){
	if (timer_number == 1){
			if (channel == 1) TIM1->CCR1 = Dutycyle;
			if (channel == 2) TIM1->CCR2 = Dutycyle;
	}
}
uint32_t hw_get_elapsed_time(void){
  return HAL_GetTick();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	app_uart_rx_callback();
}
