#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "mylcd.h"

void ADC_Init(void)
{
	ADMUX = (1 << REFS0) | (1 << MUX1) | (1 << MUX0); // 기준전압 5V, PF3

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // adc 설정 분주비 128
}

int ADC_Read(void) // adc값 읽어오기
{
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC))
		;
	return ADC;
}

main(void)
{
	int adc_value;
	float voltage;
	char buffer[20];
	int v_int, v_dec;
	unsigned char num_fnd; // 7세그먼트에 띄울 숫자 (0~9)

	PortInit();
	LCD_Init();
	ADC_Init();

	DDRC |= 0x0F;

	while (1)
	{
		adc_value = ADC_Read();
		voltage = adc_value * (5.0 / 1024.0); // 전압계산 기준 5V

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

		PORTC = (PORTC & 0xF0) | (num_fnd & 0x0F); // 7seg decoder pc0~pc3

		v_int = (int)voltage;					// 전압 정수부
		v_dec = (int)((voltage - v_int) * 100); // 전압 실수부

		LCD_Clear();

		sprintf(buffer, "Voltage: %d.%02dV", v_int, v_dec);
		LCD_pos(0, 0);
		LCD_STR(buffer);

		sprintf(buffer, "Value: %d", adc_value);
		LCD_pos(0, 1);
		LCD_STR(buffer);

		_delay_ms(200);
	}
	return 0;
}