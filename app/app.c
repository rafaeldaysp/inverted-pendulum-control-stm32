/*
 * app.c
 *
 *  Created on: Jun 13, 2023
 *      Author: Rafael
 */

#include "hw.h"
#include "main.h"
#include <string.h>

#define ENCODER_PENDULUM_TIMER_NUMBER 2
#define ENCODER_CART_TIMER_NUMBER 3
#define MOTOR_PWM_TIMER_NUMBER 4
#define MOTOR_PWM_TIMER_CHANNEL_1 TIM_CHANNEL_3
#define MOTOR_PWM_TIMER_CHANNEL_2 TIM_CHANNEL_4
#define UART_NUMBER 1
#define SAMPLE_TIME_MS 20
#define SAMPLE_TIME_INVERSE 50

int16_t counter_pendulum;
int16_t counter_cart;
int16_t counter_pendulum_velocity;
int16_t counter_cart_velocity;
int16_t last_counter_pendulum;
int16_t last_counter_cart;

uint8_t buffer_header[] = "#";
uint8_t tx_buffer[9];

uint8_t rx_buffer[2];
int16_t rx_value;
uint16_t rx_pwm_value;

uint32_t motor_pwm_clockwise;
uint32_t motor_pwm_counterclockwise;

uint32_t time_diff = 0;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	rx_value = rx_buffer[0] | (rx_buffer[1] << 8);
	motor_pwm_clockwise = 0;
	motor_pwm_counterclockwise = 0;
	if (rx_value < 0){
		rx_pwm_value = (uint32_t)(rx_value*-1);
		motor_pwm_counterclockwise = rx_pwm_value;
	}
	else motor_pwm_clockwise = (uint32_t)rx_value;


}

void update_motor_pwm(uint32_t pwm_clockwise, uint32_t pwm_counterclockwise){
	if (counter_pendulum < 900 || counter_pendulum > 1100) {
		motor_pwm_clockwise = 0;
		motor_pwm_counterclockwise = 0;
	}
	set_PWM_Dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_1, pwm_clockwise);
	set_PWM_Dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_2, pwm_counterclockwise);

}

void update_tx_buffer(int16_t *counter_cart, int16_t *counter_pendulum, int16_t *counter_cart_velocity, int16_t *counter_pendulum_velocity){
	memcpy(tx_buffer, buffer_header, 1);
	memcpy(tx_buffer + 1, (uint8_t *)counter_cart, 2);
	memcpy(tx_buffer + 3, (uint8_t *)counter_pendulum, 2);
	memcpy(tx_buffer + 5, (uint8_t *)counter_cart_velocity, 2);
	memcpy(tx_buffer + 7, (uint8_t *)counter_pendulum_velocity, 2);
}

void app_setup(){
	Encoder_Start(ENCODER_PENDULUM_TIMER_NUMBER);
	Encoder_Start(ENCODER_CART_TIMER_NUMBER);
	PWM_Start(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_1);
	PWM_Start(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_2);
	set_PWM_Dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_1, 0);
	set_PWM_Dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_2, 0);
	HAL_UART_Receive_DMA(&huart1, rx_buffer, sizeof(rx_buffer));

}

void app_loop(){
	static uint32_t current_step_time = 0;
	time_diff = get_Elapsed_Time() - current_step_time;
	update_motor_pwm(motor_pwm_clockwise, motor_pwm_counterclockwise);
	counter_cart = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
	counter_pendulum = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
	if (time_diff > SAMPLE_TIME_MS){
		counter_cart_velocity = (counter_cart - last_counter_cart) * SAMPLE_TIME_INVERSE;
		counter_pendulum_velocity = (counter_pendulum - last_counter_pendulum) * SAMPLE_TIME_INVERSE;
		update_tx_buffer(&counter_cart, &counter_pendulum, &counter_cart_velocity, &counter_pendulum_velocity);
		last_counter_cart = counter_cart;
		last_counter_pendulum = counter_pendulum;
		HAL_UART_Transmit_IT(&huart1, tx_buffer, sizeof(tx_buffer));
		current_step_time = get_Elapsed_Time();
	}
}
