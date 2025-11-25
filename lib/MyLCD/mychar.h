#ifndef MYCHAR_H_
#define MYCHAR_H_

#include <avr/io.h>
#include "mylcd.h"

// 왼쪽하트
unsigned char heart_L[8] = {
    0b00110, //   **
    0b01111, //  ****
    0b11111, // *****
    0b11111, // *****
    0b01111, //  ****
    0b00111, //   ***
    0b00011, //    **
    0b00001  //     *
};

// 오른쪽 하트
unsigned char heart_R[8] = {
    0b01100, // **
    0b11110, // ****
    0b11111, // *****
    0b11111, // *****
    0b11110, // ****
    0b11100, // ***
    0b11000, // **
    0b10000  // *
};

unsigned char star[8] = {
    0b00000, //
    0b00100, //    *
    0b01110, //   ***
    0b11111, //  *****
    0b01110, //   ***
    0b10101, //  * *
    0b00000, //
    0b00000  //
};

unsigned char smile[8] = {
    0b00000, //
    0b01010,
    0b01010, //   ***
    0b00000,
    0b00000,
    0b10001,
    0b01010, //
    0b00100  //
};

// 레지스터에 등록 최대 7개 작성필요
void Init_MyChars(void)
{
    LCD_CreateChar(0, heart_L); // 좌하트
    LCD_CreateChar(1, heart_R); // 우하트
    LCD_CreateChar(2, star);    // 별 ★
    LCD_CreateChar(3, smile);   // 스마일
}

#endif /* MYCHAR_H_ */