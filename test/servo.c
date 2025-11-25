#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int state = 0;
unsigned int angles[8] = {1300, 2300, 3400, 4300, 5000, 4300, 3400, 2300};

ISR(INT0_vect)
{
    cli();
    _delay_ms(50);

    state++;
    if (state > 7)
    {
        state = 0;
    }

    OCR1A = angles[state];

    sei();
    EIFR = (1 << INTF0);
}

main()
{
    SREG |= 0x80;

    DDRB |= (1 << DDB5);

    DDRD &= ~(1 << DDD0);
    PORTD |= (1 << PD0);

    EICRA = (1 << ISC01);
    EIMSK |= (1 << INT0);

    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12); // 14번
    TCCR1A |= (1 << COM1A1);              // 토글모드??

    TCCR1B |= (1 << CS11);

    ICR1 = 40000; // 주기

    OCR1A = angles[state];

    sei();

    while (1)
    {
    }
}