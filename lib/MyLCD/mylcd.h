
#ifndef MYLCD_H_
#define MYLCD_H_

#include <avr/io.h>
#include <util/delay.h>

#define LCD_DATA_PORT PORTA
#define LCD_DATA_DDR DDRA

#define LCD_RS_PORT PORTC
#define LCD_RS_DDR DDRC
#define LCD_RS_PIN 7

#define LCD_RW_PORT PORTC
#define LCD_RW_DDR DDRC
#define LCD_RW_PIN 6

#define LCD_E_PORT PORTC
#define LCD_E_DDR DDRC
#define LCD_E_PIN 5

void PortInit(void);
void LCD_Comm(char ch);
void LCD_Data(char ch);
void LCD_CHAR(char c);
void LCD_STR(const char *str);
void LCD_pos(unsigned char x, unsigned char y);
void LCD_Clear(void);
void LCD_Init(void);
void LCD_CreateChar(unsigned char location, unsigned char *pattern);

void PortInit(void) //port PA
{
    LCD_DATA_DDR = 0xFF;

    LCD_RS_DDR |= (1 << LCD_RS_PIN);
    LCD_RW_DDR |= (1 << LCD_RW_PIN);
    LCD_E_DDR |= (1 << LCD_E_PIN);
}


void LCD_Data(char ch)
{
    LCD_RS_PORT |= (1 << LCD_RS_PIN);
    LCD_RW_PORT &= ~(1 << LCD_RW_PIN);

    LCD_DATA_PORT = ch; 
    _delay_us(5); 

    LCD_E_PORT |= (1 << LCD_E_PIN);
    _delay_us(10); 
    LCD_E_PORT &= ~(1 << LCD_E_PIN);
    _delay_us(50); 
}


void LCD_Comm(char ch)
{
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
    LCD_RW_PORT &= ~(1 << LCD_RW_PIN);

    LCD_DATA_PORT = ch;
    _delay_us(5);

    LCD_E_PORT |= (1 << LCD_E_PIN); 
    _delay_us(10);
    LCD_E_PORT &= ~(1 << LCD_E_PIN); 
    _delay_us(50);
}

void LCD_CHAR(char c) { LCD_Data(c); }

void LCD_STR(const char *str)
{
    while (*str != 0)
    {
        LCD_CHAR(*str);
        str++;
    }
}

void LCD_pos(unsigned char x, unsigned char y)
{
    LCD_Comm(0x80 | (x + y * 0x40));
}

void LCD_Clear(void)
{
    LCD_Comm(0x01);
    _delay_ms(2);
}

void LCD_Init(void)
{

    MCUCSR |= (1 << JTD);
    MCUCSR |= (1 << JTD);

    _delay_ms(50);
    LCD_Comm(0x38);
    _delay_ms(1);
    LCD_Comm(0x38);
    _delay_us(100);
    LCD_Comm(0x38);

    LCD_Comm(0x08);
    LCD_Clear();
    LCD_Comm(0x06);
    LCD_Comm(0x0C);
}

void LCD_CreateChar(unsigned char location, unsigned char *pattern)
{
    location &= 0x07;
    LCD_Comm(0x40 | (location << 3));
    for (int i = 0; i < 8; i++)
    {
        LCD_Data(pattern[i]);
    }
}

#endif 