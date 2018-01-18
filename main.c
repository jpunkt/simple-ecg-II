/*
 * Simple-ECG Version 0.1
 * Created by jpunkt on 03-01-2018
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "u8g2/u8g2.h"
#include "lib/u8g2callback.h"


#define DISP_WIDTH 240
#define DISP_HEIGHT 128

#define FRAME_X 4
#define FRAME_Y 12

#define FRAME_WIDTH (DISP_WIDTH - FRAME_X)
#define FRAME_HEIGHT (DISP_HEIGHT - FRAME_Y)

u8g2_t u8g2;

int16_t valbuffer[FRAME_WIDTH];
int16_t values[FRAME_WIDTH];
int16_t buffer[8];

uint8_t b_k = 0;
uint8_t b_i = 0;

/*
 * Discrete sin function for testing w/o analog input
 */
uint16_t discFun(uint16_t t) {
    double A = 512;
    double omega_t = t * 5 * M_PI / (232 * 4);
    return (uint16_t) (A * cos(omega_t - M_PI) + 512);
}

/*
 * Scale ADC values for display
 */
uint16_t transformY(uint16_t y) {
    uint16_t tmp = 140 - (y / 6);
    return (tmp > 124 || tmp < 9) ? 124 : tmp;
}

/*
 * Timer interrupt
 */
ISR(TIMER0_COMPA_vect) {
    //buffer[b_k] = discFun(glob_t);  // For debugging
    buffer[b_k] = ADC;
    b_k = (b_k < 7) ? b_k+1 : 0;

    if (0 == b_k) {
        valbuffer[b_i] = transformY((buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7]) / 8);
        b_i = (b_i < FRAME_WIDTH) ? b_i + 1 : 0;
    }
}

/*
 * Initialize
 */
void init(void) {
    // Set timers
    TCCR0A |= (1 << WGM01);     // Set the Timer Mode to CTC
    OCR0A = 15;                 // Set to 15 (= 5ms @ prescaler 1024)
    TIMSK0 |= (1 << OCIE0A);    // Set the ISR COMPA vect

    sei();         //enable interrupts

    TCCR0B |= (1 << CS02)| (0 << CS01)| (1 << CS00); // Set prescaler to 101 (=1024)

    // Set up analog input
    ADMUX |= (1 << REFS0);                  // reference voltage on AVCC, for external reference use 0 and 0 for REFS0 and REFS1
    ADCSRA |= (1<<ADPS2) | (1 << ADPS1) | (1 << ADPS0);     // ADC clock prescaler / 128 --> 16000000/128=125kHz which is between 50 and 200kHz

    ADCSRA |= (1 << ADEN);                                  // enable ADC
    ADCSRA |= (1 << ADATE);                                 // auto-trigger enable
    ADCSRA |= (1 << ADSC);                                  // start first conversion

    // Set up display
    u8g2_Setup_t6963_240x128_2(&u8g2, U8G2_R0, u8x8_byte_8bit_8080mode, uC_specific_atmega328p);  // init u8g2 structure
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    u8g2_ClearDisplay(&u8g2);

}

/*
 * Draw a fancy frame on the screen
 */
void drawFrame(void) {
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    u8g2_DrawStr(&u8g2, (DISP_WIDTH / 2) - (15*5/2), 8, "Simple ECG v0.1");

    u8g2_DrawFrame(&u8g2, 2, 10, FRAME_WIDTH, FRAME_HEIGHT);
}

/*
 * Draw the data points
 */
void drawData() {
    for (int i = 0; i < FRAME_WIDTH; ++i) {

        if(i == 0 || values[i-1] == 0) {
            u8g2_DrawPixel(&u8g2, i+2, values[i]);
        } else {
            u8g2_DrawLine(&u8g2, i+2, values[i], i+3, values[i-1]);
        }
    }
}


int main(void)
{
    init();

    while (1) {

        u8g2_FirstPage(&u8g2);
        do {
            drawFrame();

            drawData();

        } while (u8g2_NextPage(&u8g2));

        for (int i = 0; i < FRAME_WIDTH; ++i) {
            values[i] = valbuffer[i];
        }
    }
}
