/*
 * hw.h
 *
 *  Created on: Jun 13, 2023
 *      Author: Rafael
 */
#include "main.h"

#ifndef HW_H_
#define HW_H_


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void PWM_Start(uint32_t timer_number, uint32_t Channel);
void Encoder_Start(uint32_t timer_number);
void set_PWM_Dutycycle(uint32_t timer_number, uint32_t Channel, uint32_t Dutycyle);
uint32_t get_PWM_Dutycyle(uint32_t timer_number, uint32_t Channel);
uint32_t get_Elapsed_Time(void);
void UART_Transmit(uint32_t uart_number, uint8_t *tx_data, uint16_t size_tx_data);
uint32_t get_Encoder_Count(uint32_t encoder_number);
void UART_Receive_Start(uint32_t uart_number, uint8_t *buffer, uint16_t size);

#endif /* HW_H_ */
