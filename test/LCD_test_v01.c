#include <avr/io.h>
#include <util/delay.h>

#define LCD_WDATA PORTA // LCD 데이터 포트 정의
#define LCD_WINST PORTA
#define LCD_CTRL PORTG // LCD 제어포트 정의
#define LCD_EN 0
#define LCD_RW 1
#define LCD_RS 2

void PortInit(void)
{
	DDRA=0xFF;  // PORTA 를 출력으로
	DDRG=0x0F;  // PORTC 의 하위 4 비트를 출력으로
}

void LCD_Data(char ch) //LCD_DR 에 데이터 출력
{
	LCD_CTRL |= (1 << LCD_RS); // RS=1, =0으로 데이터 쓰기 사이클
	LCD_CTRL &= ~(1 << LCD_RW);
	LCD_CTRL |= (1 << LCD_EN);  // LCD Enable
	_delay_us(50); 
	LCD_WDATA = ch; //데이터 출력
	_delay_us(50); 
	LCD_CTRL &= ~(1 << LCD_EN); // LCD Disable
}
void LCD_Comm(char ch) // LCD IR 에 명령어 쓰기
{
	LCD_CTRL &= ~(1 << LCD_RS); // RS==0으로 명령어 쓰기 사이클
	LCD_CTRL &= ~(1 << LCD_RW);
	LCD_CTRL |= (1 << LCD_EN); // LCD Enable
	_delay_us(50); 
	LCD_WINST = ch; //명령어 쓰기
	_delay_us(50); 
	LCD_CTRL &= ~(1 << LCD_EN); // LCD Disable
}
void LCD_delay(char ms)
{
	_delay_ms(ms);
}

void LCD_CHAR(char c) //한 문자 출력
{
	LCD_Data(c);
	_delay_ms(1);
}
void LCD_STR(char *str) // 문자열 출력
{
	while(*str != 0) {
	LCD_CHAR(*str);
	str++;
	}
}
void LCD_pos(unsigned char row, unsigned char col) //LCD 포지션 설정
{
	LCD_Comm(0x80 |(row+ col*0x40)); // row 문자행, col 문자열
}
void LCD_Clear(void) //화면 클리어 1
{
	LCD_Comm(0x01);
	LCD_delay(2);
}


void LCD_Init(void) //LCD 초기화
{
	LCD_Comm(0x38); //DDRAM, 데이터 8 비트사용 LCD 2 열로 사용 6
	LCD_delay(2); 
	LCD_Comm(0x38); 
	LCD_delay(2); 
	LCD_Comm(0x38); 
	LCD_delay(2); 
	LCD_Comm(0x0e);// Display ON/OFF
	LCD_delay(2); 
	LCD_Comm(0x06); //주소 1 커서를 우측 이동 3
	LCD_delay(2); 
	LCD_Clear(); //LCD 화면 클리어
}

void main(void)
{
	char str[]="LCD Test...";
	PortInit(); //LCD 출력 포트 설정
	LCD_Init();  //LCD 초기화

	while(1) {
	LCD_pos(5,1); //LCD 포지션 5열 1행 지정
	LCD_STR(str); //문자열 str 을 LCD 출력
	LCD_delay(2);
	};
}
