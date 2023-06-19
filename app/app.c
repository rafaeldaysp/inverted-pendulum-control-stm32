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
#define MOTOR_PWM_TIMER_NUMBER 1
#define MOTOR_PWM_TIMER_CHANNEL_1 1
#define MOTOR_PWM_TIMER_CHANNEL_2 2
#define UART_NUMBER 1
#define SAMPLE_TIME_MS 20
#define SAMPLE_TIME_INVERSE 50
#define TX_BUFFER_HEADER 35

typedef struct {
	int16_t position_count;
	int16_t angle_count;
	int16_t velocity_count;
	int16_t angular_velocity_count;
} States;

States states;
uint8_t tx_buffer[9];
uint8_t rx_buffer[2];
uint32_t motor_pwm_clockwise;
uint32_t motor_pwm_counterclockwise;

void app_uart_rx_callback(void){
	int16_t rx_value = rx_buffer[0] | (rx_buffer[1] << 8);
	motor_pwm_clockwise = 0;
	motor_pwm_counterclockwise = 0;
	if (rx_value < 0) motor_pwm_counterclockwise = (uint32_t)(rx_value * -1);
	else motor_pwm_clockwise = (uint32_t)rx_value;
}

void app_update_motor_pwm(){
	if (states.angle_count < 850 || states.angle_count > 1150) {
		motor_pwm_clockwise = 0;
		motor_pwm_counterclockwise = 0;
	}
	hw_set_pwm_dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_1, motor_pwm_clockwise);
	hw_set_pwm_dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_2, motor_pwm_counterclockwise);
}

void app_setup(void){
	hw_encoder_start(ENCODER_PENDULUM_TIMER_NUMBER);
	hw_encoder_start(ENCODER_CART_TIMER_NUMBER);
	hw_pwm_start(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_1);
	hw_pwm_start(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_2);
	hw_uart_receive_dma(UART_NUMBER, rx_buffer, sizeof(rx_buffer));
	tx_buffer[0] = TX_BUFFER_HEADER;
}
void app_loop(void){
	static int16_t last_angle_count;
	static int16_t last_position_count;
	static uint32_t step_time_ref = 0;

	app_update_motor_pwm();
	states.position_count = (int16_t)hw_get_encoder_count(ENCODER_CART_TIMER_NUMBER);
	int16_t pendulum_encoder_count = (int16_t)hw_get_encoder_count(ENCODER_PENDULUM_TIMER_NUMBER);
	states.angle_count = pendulum_encoder_count % 1000 - 1000 * ((pendulum_encoder_count % 2000) / 1000);

	if (hw_get_elapsed_time() - step_time_ref > SAMPLE_TIME_MS){
		states.velocity_count = (states.position_count - last_position_count) * SAMPLE_TIME_INVERSE;
		states.angular_velocity_count = (states.angle_count - last_angle_count) * SAMPLE_TIME_INVERSE;
		memcpy(tx_buffer + 1, &states, sizeof(states));
		hw_uart_transmit_it(UART_NUMBER, tx_buffer, sizeof(tx_buffer));
		last_position_count = states.position_count;
		last_angle_count = states.angle_count;
		step_time_ref = hw_get_elapsed_time();
	}
}
