/*
 * usb.c
 *
 *  Created on: Jun 19, 2023
 *      Author: Rafael
 */

#include "usbd_cdc_if.h"
#include "app.h"

void usb_transmit(uint8_t *buffer, uint16_t size){
	CDC_Transmit_FS(buffer, size);
}
