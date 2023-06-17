/*
 * hw.c
 *
 *  Created on: Jun 13, 2023
 *      Author: Rafael
 */

#include "hw.h"
#include "main.h"
#include "app.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;

void PWM_Start(uint32_t timer_number, uint32_t Channel){
	if (timer_number == 4) HAL_TIM_PWM_Start(&htim4, Channel);

}
void Encoder_Start(uint32_t timer_number){
	if (timer_number == 2) HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
	if (timer_number == 3) HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
}

uint32_t get_Encoder_Count(uint32_t encoder_number){
	if(encoder_number == 2) return __HAL_TIM_GET_COUNTER(&htim2);
	if(encoder_number == 3) return __HAL_TIM_GET_COUNTER(&htim3);
	return 0;
}

void UART_Receive_Start(uint32_t uart_number, uint8_t *buffer, uint16_t size){
	if (uart_number == 1) HAL_UARTEx_ReceiveToIdle_IT(&huart1, buffer, size);
}

void UART_Transmit(uint32_t uart_number, uint8_t *tx_data, uint16_t size_tx_data){
	if(uart_number == 1) HAL_UART_Transmit_IT(&huart1, tx_data, size_tx_data);
}


void set_PWM_Dutycycle(uint32_t timer_number, uint32_t Channel, uint32_t Dutycyle){
	if (timer_number == 4){
		if (Channel == TIM_CHANNEL_3) TIM4->CCR3 = Dutycyle;
		if (Channel == TIM_CHANNEL_4) TIM4->CCR4 = Dutycyle;
	}
}

uint32_t get_PWM_Dutycyle(uint32_t timer_number, uint32_t Channel){
	uint32_t PWM_Dutycycle = 0;
	if (timer_number == 4 && Channel == TIM_CHANNEL_1) PWM_Dutycycle = TIM4->CCR1;

	return PWM_Dutycycle;
}

uint32_t get_Elapsed_Time(void){
  return HAL_GetTick();
}

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
//	uint32_t count = __HAL_TIM_GET_COUNTER(htim);
//	TIM_HandleTypeDef *htim_callback = htim;
//	if(htim == &htim2) Encoder_Callback_Handler(2, count);
//	else if(htim == &htim3) Encoder_Callback_Handler(3, count);
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//	if (huart == huart1) HAL_UART_Receive_IT(&huart1, rx_buffer, Size)
//}
