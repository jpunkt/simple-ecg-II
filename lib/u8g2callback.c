//
// Created by jpunkt on 1/3/18.
//

#include "u8g2callback.h"

uint8_t uC_specific_atmega328p(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:    // called once during init phase of u8g2/u8x8
                                              // can be used to setup pins
            PORT_AS_OUTPUT(GLCD_DATA_DDR);
            PORT_AS_OUTPUT(GLCD_CTRL_DDR);

            GLCD_CTRL_PORT |= ((0 << GLCD_WR) | (0 << GLCD_CE) | (0 << GLCD_CD) | (0 << GLCD_RESET));
            GLCD_DATA_PORT = 0xFF;

            break;
        case U8X8_MSG_DELAY_NANO:             // delay arg_int * 1 nano second
                                              //_delay_us((double) arg_int / 1000);
            //NOP, one cycle is 188ns
            break;
        case U8X8_MSG_DELAY_100NANO:          // delay arg_int * 100 nano seconds
                                              // _delay_us((double) arg_int / 10);
            //NOP
            break;
        case U8X8_MSG_DELAY_10MICRO:        // delay arg_int * 10 micro seconds
            for (int i = 0; i < (10 * arg_int); ++i) {
                _delay_us(1);
            }
            break;
        case U8X8_MSG_DELAY_MILLI:            // delay arg_int * 1 milli second
            for (int i = 0; i < arg_int; ++i) {
                _delay_ms(1);
            }
            break;
        case U8X8_MSG_DELAY_I2C:                // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            //NOP
            break;                            // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_D0:                // D0 or SPI clock pin: Output level in arg_int
                                              //case U8X8_MSG_GPIO_SPI_CLOCK:
            PIN_SET(GLCD_DATA_PORT, 0, arg_int);
            break;
        case U8X8_MSG_GPIO_D1:                // D1 or SPI data pin: Output level in arg_int
                                              //case U8X8_MSG_GPIO_SPI_DATA:
            PIN_SET(GLCD_DATA_PORT, 1, arg_int);
            break;
        case U8X8_MSG_GPIO_D2:                // D2 pin: Output level in arg_int
            PIN_SET(GLCD_DATA_PORT, 2, arg_int);
            break;
        case U8X8_MSG_GPIO_D3:                // D3 pin: Output level in arg_int
            PIN_SET(GLCD_DATA_PORT, 3, arg_int);
            break;
        case U8X8_MSG_GPIO_D4:                // D4 pin: Output level in arg_int
            PIN_SET(GLCD_DATA_PORT, 4, arg_int);
            break;
        case U8X8_MSG_GPIO_D5:                // D5 pin: Output level in arg_int
            PIN_SET(GLCD_DATA_PORT, 5, arg_int);
            break;
        case U8X8_MSG_GPIO_D6:                // D6 pin: Output level in arg_int
            PIN_SET(GLCD_DATA_PORT, 6, arg_int);
            break;
        case U8X8_MSG_GPIO_D7:                // D7 pin: Output level in arg_int
            PIN_SET(GLCD_DATA_PORT, 7, arg_int);
            break;
        case U8X8_MSG_GPIO_E:                // E/WR pin: Output level in arg_int
            PIN_SET(GLCD_CTRL_PORT, GLCD_WR, arg_int);
            break;
        case U8X8_MSG_GPIO_CS:                // CS (chip select) pin: Output level in arg_int
            //PIN_SET(GLCD_CTRL_PORT, GLCD_CE, arg_int);
            break;
        case U8X8_MSG_GPIO_DC:                // DC (data/cmd, A0, register select) pin: Output level in arg_int
            PIN_SET(GLCD_CTRL_PORT, GLCD_CD, arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:            // Reset pin: Output level in arg_int
            PIN_SET(GLCD_CTRL_PORT, GLCD_RESET, arg_int);
            break;
        case U8X8_MSG_GPIO_CS1:                // CS1 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS2:                // CS2 (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_I2C_CLOCK:        // arg_int=0: Output low at I2C clock pin
            break;                            // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:            // arg_int=0: Output low at I2C data pin
            break;                            // arg_int=1: Input dir with pullup high for I2C data pin
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1);            // default return value
            break;
    }
    return 1;
}