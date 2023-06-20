/*
 * app.c
 *
 *  Created on: Jun 13, 2023
 *      Author: Rafael
 */

#include "hw.h"
#include "main.h"
#include <string.h>
#include "usb.h"

#define ENCODER_PENDULUM_TIMER_NUMBER 2
#define ENCODER_CART_TIMER_NUMBER 3
#define MOTOR_PWM_TIMER_NUMBER 1
#define MOTOR_PWM_TIMER_CHANNEL_1 1
#define MOTOR_PWM_TIMER_CHANNEL_2 2
#define UART_NUMBER 1
#define SAMPLE_TIME_MS 10
#define SAMPLE_TIME_INVERSE 100
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


void app_usb_rx_callback(uint8_t* buffer, size_t size){
	memset(rx_buffer, '\0', sizeof(rx_buffer));
	memcpy(rx_buffer, buffer, sizeof(rx_buffer));
	memset(buffer, '\0', size);
	int16_t rx_value = rx_buffer[0] | (rx_buffer[1] << 8);
	motor_pwm_clockwise = 0;
	motor_pwm_counterclockwise = 0;
	if (rx_value < 0) motor_pwm_counterclockwise = (uint32_t)(rx_value * -1);
	else motor_pwm_clockwise = (uint32_t)rx_value;
}

void app_update_motor_pwm(){

	if (states.angle_count > 1150 || states.angle_count < 850) {
		motor_pwm_clockwise = 0;
		motor_pwm_counterclockwise = 0;
	}
	hw_set_pwm_dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_1, motor_pwm_clockwise);
	hw_set_pwm_dutycycle(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_2, motor_pwm_counterclockwise);
}

int16_t app_calc_angle(int16_t count){
	if (count < 0)
		return count % 2000 + 2000;
	return count % 2000;
}

void app_setup(void){
	hw_encoder_start(ENCODER_PENDULUM_TIMER_NUMBER);
	hw_encoder_start(ENCODER_CART_TIMER_NUMBER);
	hw_pwm_start(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_1);
	hw_pwm_start(MOTOR_PWM_TIMER_NUMBER, MOTOR_PWM_TIMER_CHANNEL_2);
	tx_buffer[0] = TX_BUFFER_HEADER;
}
void app_loop(void){
	static int16_t last_angle_count;
	static int16_t last_position_count;
	static uint32_t step_time_ref = 0;

	app_update_motor_pwm();
	states.position_count = (int16_t)hw_get_encoder_count(ENCODER_CART_TIMER_NUMBER);
	states.angle_count = app_calc_angle((int16_t)hw_get_encoder_count(ENCODER_PENDULUM_TIMER_NUMBER));
	if (hw_get_elapsed_time() - step_time_ref > SAMPLE_TIME_MS){
		states.velocity_count = (states.position_count - last_position_count) * SAMPLE_TIME_INVERSE;
		states.angular_velocity_count = (states.angle_count - last_angle_count) * SAMPLE_TIME_INVERSE;
		memcpy(tx_buffer + 1, &states, sizeof(states));
		usb_transmit(tx_buffer, sizeof(tx_buffer));
		last_position_count = states.position_count;
		last_angle_count = states.angle_count;
		step_time_ref = hw_get_elapsed_time();
	}
}
