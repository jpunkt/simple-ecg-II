#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "u8g2/u8g2.h"
#include "lib/u8g2callback.h"


#define DISP_WIDTH 240
#define DISP_HEIGHT 128

#define FRAME_X 3
#define FRAME_Y 12

#define FRAME_WIDTH (DISP_WIDTH - FRAME_X)
#define FRAME_HEIGHT (DISP_HEIGHT - FRAME_Y)

u8g2_t u8g2;

int16_t values[FRAME_WIDTH][2];
int16_t buffer[8];

uint16_t glob_t = 0;
uint8_t b_k = 0;
uint8_t b_i = 0;

uint16_t adcValue10bit;

uint16_t discFun(uint16_t t) {
    double A = 512;
    double omega_t = t * 5 * M_PI / (232 * 4);
    return (uint16_t) (A * cos(omega_t - M_PI) + 512);
}

uint16_t transformY(uint16_t y) {
    return 140 - (y / 6);
}

ISR(TIMER0_COMPA_vect) {
    //buffer[b_k] = transformY(discFun(glob_t));  //TODO set from analog in
    buffer[b_k] = ADC;
    b_k = (b_k < 7) ? b_k+1 : 0;
    glob_t = (glob_t < 232 * 4) ? glob_t+1 : 0;
    //glob_t++;

    if (0 == glob_t % 8) {
        values[b_i][0] = transformY((buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7]) / 8);
        values[b_i][1] = (buffer[7] - buffer[0]) / 10;
        b_i = (b_i < 232) ? b_i + 1 : 0;
    }
}

void init(void) {
    // Set timers
    TCCR0A |= (1 << WGM01); // Set the Timer Mode to CTC
    OCR0A = 20;  // TODO Set to 15 (= 5ms @ prescaler 1024)
    TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

    sei();         //enable interrupts

    TCCR0B |= (1 << CS02)| (0 << CS01)| (1 << CS00); //TODO set prescaler to 101 (=1024)

    // Set analog input
    ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC, for external reference use 0 and 0 for REFS0 and REFS1 */
    ADCSRA |= (1<<ADPS2) | (1 << ADPS1) | (1 << ADPS0);     /* ADC clock prescaler / 128 --> 16000000/128=125kHz which is between 50 and 200kHz*/

    //ADMUX |= (1 << ADLAR);     /* left-adjust result, return only 8 bits */

    ADCSRA |= (1 << ADEN);                                 /* enable ADC */
    ADCSRA |= (1 << ADATE);                       /* auto-trigger enable */
    ADCSRA |= (1 << ADSC);                     /* start first conversion */

    // Set up display
    u8g2_Setup_t6963_240x128_2(&u8g2, U8G2_R0, u8x8_byte_8bit_8080mode, uC_specific_atmega328p);  // init u8g2 structure
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    u8g2_ClearDisplay(&u8g2);
}

void drawFrame(void) {
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    u8g2_DrawStr(&u8g2, (DISP_WIDTH / 2) - (15*5/2), 8, "Simple ECG v0.1");

    char *tempstr = "     ";
    sprintf(tempstr, "t:%3i", glob_t);

    u8g2_DrawStr(&u8g2, (DISP_WIDTH - 25), 8, tempstr);

    //u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawFrame(&u8g2, 2, 10, FRAME_WIDTH, FRAME_HEIGHT);
}


void drawData() {
    for (int i = 0; i < sizeof(values); ++i) {

        /*uint8_t translen = ceil(fabs(values[i][1]) / 10);
        uint8_t transy = 0;

        if (values[i][1] < -1) {
            transy = transformY(values[i][0]) + translen / 2;
        } else if (values[i][1] > 1) {
            transy = transformY(values[i][0]) - translen / 2;
        } else {
            transy = transformY(values[i][0]);
        }*/
        //uint8_t translen = ceil(fabs(values[i][1]) / 10);
        //uint8_t transy = transformY(values[i][0]);
        //uint8_t y1 = values[i][0];
        //if (values[i][1] <= 1) {
            u8g2_DrawPixel(&u8g2, i+3, values[i][0]);
        //} else {
        //    u8g2_DrawLine(&u8g2, i+3, values[i][0], i+3, values[i][0]+values[i][1]);
        //}
        //for (int j = 0; j < translen; ++j) {
            //u8g2_DrawVLine(&u8g2, i+3, transformY(values[i][0])-translen/2, translen);
        //}
    }
}


int main(void)
{
    init();

    while (1) {

        u8g2_FirstPage(&u8g2);
        do {
            drawFrame();

            /*char *tempstr = "                                   ";
            uint8_t i = (b_i == 0) ? 0 : b_i-1;
            sprintf(tempstr, "b_i:%3i, value:%3i, len:  %3i", i, values[i][0], values[i][1]);

            u8g2_DrawStr(&u8g2, 25, 25, tempstr);
            uint8_t transy = transformY(values[i][0]);
            uint8_t translen = ceil(fabs(values[i][1]) / 10);
            sprintf(tempstr, "         trans:%3i, trans:%3i", transy, translen);

            u8g2_DrawStr(&u8g2, 25, 34, tempstr);*/
            drawData();

        } while (u8g2_NextPage(&u8g2));

    }
}
