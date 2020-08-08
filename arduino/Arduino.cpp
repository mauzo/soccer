#include <Arduino.h>
#include <avr/interrupt.h>

HardwareSerial  Serial;

void setup (void);
void loop  (void);

int
main (void)
{
    sei();
    setup();
    while (1) loop();
}
