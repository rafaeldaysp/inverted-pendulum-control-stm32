/*
 * app.h
 *
 *  Created on: Jun 13, 2023
 *      Author: Rafael
 */

#ifndef APP_H_
#define APP_H_


void app_usb_rx_callback(uint8_t* rx_buffer, uint16_t size);
void app_uart_rx_callback(void);
void app_setup(void);
void app_loop(void);


#endif /* APP_H_ */
