#include <avr/io.h>
#include <util/delay.h>
#include "easyavr.h"

#define LED_PORT PORTB
#define LED_PIN 5
#define LED_DDR DDRB

int main(void)
{
    //DDRB = 0xFF;

    PIN_AS_OUTPUT(LED_DDR, LED_PIN);

    while (1) {
        PIN_ON(LED_PORT, LED_PIN);
        _delay_ms(1000);
        PIN_OFF(LED_PORT, LED_PIN);
        _delay_ms(500);
    }
}
