#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "mylcd.h"

int LED_sec = 0;

ISR(TIMER0_COMP_vect)
{
	static int count = 0;

	if (LED_sec == 0)
	{
		PORTB |= (1 << PB0);
		count = 0;
	}
	else
	{

		count++;
		if (count >= LED_sec)
		{
			PORTC ^= (1 << PC4); // 토글(^)
			count = 0;			 // 카운트 초기화
		}
	}
}

void ADC_Init(void)
{
	ADMUX = (1 << MUX1) | (1 << MUX0);												 // pf3으로 변경
	ADCSRA = (1 << ADEN) | (1 << ADFR) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 분주비 128 & adc &freerunning 활성화
	ADCSRA |= (1 << ADSC);															 // 변환시작
}

int ADC_Read(void) // ADCL&ADCH를 합쳐서 ADC값 반환
{
	unsigned char low_byte = ADCL;
	unsigned char high_byte = ADCH;
	int result = (high_byte << 8) | low_byte;

	return result;
}

int main(void)
{
	int adc_value;
	float voltage;
	char buffer[20];
	int v_int, v_dec;
	unsigned char num_fnd;

	PortInit();
	LCD_Init();
	ADC_Init();

	DDRC |= 0x0F;
	DDRC |= (1 << PC4);

	TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS01) | (1 << CS00); // CTC모드 분주비 1024

	OCR0 = 155;

	TIMSK |= (1 << OCIE0);

	sei();

	while (1)
	{
		adc_value = ADC_Read();
		voltage = adc_value * (5.0 / 1024.0);

		if (voltage >= 4.0)
		{
			LED_sec = 0;
		}
		else if (voltage >= 3.0)
		{
			LED_sec = 10;
		}
		else if (voltage >= 2.0)
		{
			LED_sec = 25;
		}
		else if (voltage >= 1.0)
		{
			LED_sec = 50;
		}
		else
		{
			LED_sec = 100;
		}

		if (adc_value >= 900)
			num_fnd = 9;
		else if (adc_value >= 800)
			num_fnd = 8;
		else if (adc_value >= 700)
			num_fnd = 7;
		else if (adc_value >= 600)
			num_fnd = 6;
		else if (adc_value >= 500)
			num_fnd = 5;
		else if (adc_value >= 400)
			num_fnd = 4;
		else if (adc_value >= 300)
			num_fnd = 3;
		else if (adc_value >= 200)
			num_fnd = 2;
		else if (adc_value >= 100)
			num_fnd = 1;
		else
			num_fnd = 0;

		PORTC = (PORTC & 0xF0) | (num_fnd & 0x0F);

		v_int = (int)voltage;
		v_dec = (int)((voltage - v_int) * 1000);

		LCD_Clear();

		sprintf(buffer, "Voltage: %d.%03dV", v_int, v_dec);
		LCD_pos(0, 0);
		LCD_STR(buffer);

		sprintf(buffer, "Value: %d", adc_value);
		LCD_pos(0, 1);
		LCD_STR(buffer);

		_delay_ms(200);
	}
	return 0;
}