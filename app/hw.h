/*
 * hw.h
 *
 *  Created on: Jun 13, 2023
 *      Author: Rafael
 */
#include "main.h"

#ifndef HW_H_
#define HW_H_


void hw_pwm_start(uint8_t timer_number, uint8_t channel);
void hw_encoder_start(uint8_t timer_number);
void hw_set_pwm_dutycycle(uint8_t timer_number, uint32_t channel, uint32_t Dutycyle);
uint32_t hw_get_elapsed_time(void);
uint32_t hw_get_encoder_count(uint8_t encoder_number);

#endif /* HW_H_ */
