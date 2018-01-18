/*
 * Version 0.1
 * Created by jpunkt on 03-01-2018
 */

#ifndef SIMPLE_ECG_U8G2CALLBACK_H
#define SIMPLE_ECG_U8G2CALLBACK_H

#include <stdint.h>
#include "easyavr.h"
#include "util/delay.h"
#include <u8g2/u8g2.h>

// data port
#define GLCD_DATA_PORT 	PORTD
#define GLCD_DATA_PIN	PIND
#define GLCD_DATA_DDR	DDRD
// control port
#define GLCD_CTRL_PORT	PORTB
#define GLCD_CTRL_PIN	PINB
#define GLCD_CTRL_DDR	DDRB
// control signals
#define GLCD_WR			PB0
//#define GLCD_RD			PB1
#define GLCD_CE			PB3
#define GLCD_CD			PB2
#define GLCD_RESET		PB4
//#define GLCD_FS			PB5

uint8_t uC_specific_atmega328p(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif //SIMPLE_ECG_U8G2CALLBACK_H
