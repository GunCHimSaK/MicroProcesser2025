#define F_CPU 16000000UL // 사용하는 크리스탈 속도에 맞춰 변경 (예: 16MHz)
#include <avr/io.h>
#include <util/delay.h>
#include "mylcd.h"

int main(void)
{
    char str[] = "LCD Test...";
    PortInit();
    LCD_Init();

    // 정적 텍스트는 루프 밖에서 한 번만 출력
    LCD_pos(0, 0); // 첫 번째 줄, 첫 칸
    LCD_STR("Hello");

    LCD_pos(5, 1); // 두 번째 줄, 6번째 칸 (index 5)
    LCD_STR(str);

    while (1)
    {
        // 여기서는 센서 값 갱신 등 반복 작업 수행
    }
    return 0;
}