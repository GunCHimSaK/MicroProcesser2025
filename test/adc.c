#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "mylcd.h"

void ADC_Init(void)
{
    // [변경됨] 기준전압 AVCC(5V) + 입력 채널 ADC3 (PF3) 선택
    ADMUX = (1 << REFS0) | (1 << MUX1) | (1 << MUX0);

    // ADC 켜기, 분주비 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

int ADC_Read(void)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

int main(void)
{
    int adc_value;
    float voltage;
    char buffer[20]; 
    int v_int, v_dec;

    PortInit();
    LCD_Init();
    ADC_Init();

    while (1)
    {
        adc_value = ADC_Read();
        voltage = adc_value * (5.0 / 1024.0);

        v_int = (int)voltage; 
        v_dec = (int)((voltage - v_int) * 100); 

        LCD_Clear();

        sprintf(buffer, "Value: %d", adc_value);
        LCD_pos(0, 0);
        LCD_STR(buffer);

        sprintf(buffer, "Volt : %d.%02d V", v_int, v_dec);
        LCD_pos(0, 1);
        LCD_STR(buffer);

        _delay_ms(200); 
    }
    return 0;
}