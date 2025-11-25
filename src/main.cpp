#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mylcd.h"
#include "mychar.h"

int state = 0;
int update_flag = 1;

ISR(INT0_vect)
{
	cli();
	_delay_ms(50);

	state++;
	if (state > 3)
	{
		state = 0;
	}

	update_flag = 1;

	EIFR |= (1 << INTF0);
	sei();
}

main()
{
	PortInit();
	LCD_Init();
	Init_MyChars();

	DDRD &= ~(1 << DDD0);
	PORTD |= (1 << PD0);

	EICRA = (1 << ISC01);
	EIMSK |= (1 << INT0);

	sei();

	while (1)
	{
		if (update_flag == 1)
		{
			update_flag = 0;

			switch (state)
			{
			case 0:
				LCD_Clear();
				break;

			case 1:
				LCD_pos(0, 0);
				LCD_STR("20221330");
				break;

			case 2:
				LCD_pos(0, 1);
				LCD_STR("HGJ");
				break;

			case 3:
				LCD_pos(3, 1);
				LCD_STR(" ");
				LCD_Data(2);
				LCD_Data(3);
				LCD_Data(0);
				LCD_Data(1);
				break;
			}
		}
	}
	return 0;
}