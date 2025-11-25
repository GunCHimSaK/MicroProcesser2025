/*
 * mylcd.h (Final Turbo & Custom Pin)
 * 1. 핀 설정: RS=PC7, RW=PC6, E=PC5
 * 2. 속도: Turbo Mode (딜레이 최소화)
 */
#ifndef MYLCD_H_
#define MYLCD_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

// ================================================================
// ★★★ 핀 설정 (PC7, PC6, PC5) ★★★
// ================================================================

// 1. 데이터 버스 (D0~D7) -> PORTA
#define LCD_DATA_PORT PORTA
#define LCD_DATA_DDR  DDRA

// 2. 제어 신호 (PORTC)
// RS -> PC7
#define LCD_RS_PORT PORTC
#define LCD_RS_DDR  DDRC
#define LCD_RS_PIN  7

// RW -> PC6
#define LCD_RW_PORT PORTC
#define LCD_RW_DDR  DDRC
#define LCD_RW_PIN  6

// E (Enable) -> PC5
#define LCD_E_PORT  PORTC
#define LCD_E_DDR   DDRC
#define LCD_E_PIN   5

// ================================================================

void PortInit(void);
void LCD_Comm(char ch);
void LCD_Data(char ch);
void LCD_CHAR(char c);
void LCD_STR(const char *str);
void LCD_pos(unsigned char x, unsigned char y);
void LCD_Clear(void);
void LCD_Init(void);
void LCD_CreateChar(unsigned char location, unsigned char *pattern);

// --- 함수 구현 ---

void PortInit(void) {
    LCD_DATA_DDR = 0xFF; // 데이터 포트 출력 설정
    
    // 제어 핀 출력 설정
    LCD_RS_DDR |= (1 << LCD_RS_PIN);
    LCD_RW_DDR |= (1 << LCD_RW_PIN);
    LCD_E_DDR  |= (1 << LCD_E_PIN);
}

// [데이터 쓰기 - 터보 모드]
void LCD_Data(char ch) {
    LCD_RS_PORT |= (1 << LCD_RS_PIN);   // RS = 1 (데이터)
    LCD_RW_PORT &= ~(1 << LCD_RW_PIN);  // RW = 0 (쓰기)
    
    LCD_DATA_PORT = ch;                 // 데이터 출력
    _delay_us(5);                       // 대기 최소화
    
    LCD_E_PORT |= (1 << LCD_E_PIN);     // E = 1
    _delay_us(10);                      // 펄스 폭 최소화
    LCD_E_PORT &= ~(1 << LCD_E_PIN);    // E = 0
    _delay_us(50);                      // 처리 대기 (50us면 충분)
}

// [명령어 쓰기 - 터보 모드]
void LCD_Comm(char ch) {
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);  // RS = 0 (명령어)
    LCD_RW_PORT &= ~(1 << LCD_RW_PIN);  // RW = 0 (쓰기)
    
    LCD_DATA_PORT = ch;
    _delay_us(5);
    
    LCD_E_PORT |= (1 << LCD_E_PIN);     // E = 1
    _delay_us(10);
    LCD_E_PORT &= ~(1 << LCD_E_PIN);    // E = 0
    _delay_us(50);
}

void LCD_CHAR(char c) { LCD_Data(c); }

void LCD_STR(const char *str) {
    while (*str != 0) {
        LCD_CHAR(*str);
        str++;
    }
}

void LCD_pos(unsigned char x, unsigned char y) {
    LCD_Comm(0x80 | (x + y * 0x40));
}

void LCD_Clear(void) {
    LCD_Comm(0x01);
    _delay_ms(2); // Clear는 2ms 유지 (필수)
}

void LCD_Init(void) {
    // JTAG 끄기 (안전장치)
    MCUCSR |= (1 << JTD);
    MCUCSR |= (1 << JTD);

    _delay_ms(50);
    LCD_Comm(0x38); _delay_ms(1);
    LCD_Comm(0x38); _delay_us(100);
    LCD_Comm(0x38);
    
    LCD_Comm(0x08);
    LCD_Clear();
    LCD_Comm(0x06);
    LCD_Comm(0x0C);
}

void LCD_CreateChar(unsigned char location, unsigned char *pattern) {
    location &= 0x07; 
    LCD_Comm(0x40 | (location << 3)); 
    for (int i = 0; i < 8; i++) {
        LCD_Data(pattern[i]);
    }
}

#endif /* MYLCD_H_ */