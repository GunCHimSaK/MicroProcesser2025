#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// [삭제됨] 74LS47이 대신 해주므로 이 배열은 필요 없습니다.
// unsigned char FND_DATA_TBL[] = ...; 

unsigned char lowValue = 0;
unsigned char highValue = 0;
unsigned int adcValue = 0;
unsigned int timer0Cnt = 0;

void print_FND(unsigned char data);

void main() {
    DDRC = 0xFF;  // 74LS47의 A,B,C,D 입력핀 연결 (PC0~PC3)
    PORTC = 0x00; 
    
    DDRG = 0x0F;  // 7세그먼트 공통 단자(Digit) 제어용
    PORTG = 0x0F; // (회로에 따라 0x00 또는 0x0F로 설정)

    TCCR0 = 0x04; // Timer0 (Prescaler 64)
    
    TIMSK = 1 << TOIE0; 
    TIFR = 1 << TOV0;   
    
    // ADC 설정 (가변저항 5V 연결 시 ADMUX=0x40 권장, 현재는 0xC0 유지)
    ADMUX = 0xC0;  
    ADCSRA = 0xA6; 
    ADCSRA |= 0x40; 
    
    sei(); 

    while(1) {
        unsigned char num;
        
        // 전압 레벨에 따라 0~9 숫자 결정
        if (adcValue >= 900) num = 9;
        else if (adcValue >= 800) num = 8;
        else if (adcValue >= 700) num = 7;
        else if (adcValue >= 600) num = 6;
        else if (adcValue >= 500) num = 5;
        else if (adcValue >= 400) num = 4;
        else if (adcValue >= 300) num = 3;
        else if (adcValue >= 200) num = 2;
        else if (adcValue >= 100) num = 1;
        else num = 0;
        
        print_FND(num); // 숫자 그대로 전달!
        _delay_ms(3);
    }
}

ISR(TIMER0_OVF_vect) {
    timer0Cnt++;
    if (timer0Cnt == 1000) { 
        timer0Cnt = 0;
    }
    
    while ((ADCSRA & 0x10) == 0x00); 
    
    lowValue = ADCL;
    highValue = ADCH;
    adcValue = (highValue << 8) | lowValue;
    
    ADCSRA |= 0x10; 
}

void print_FND(unsigned char data) {
    // [수정됨] 배열을 거치지 않고 숫자(0~9)를 바로 보냅니다.
    // 74LS47이 이 숫자를 받아서 알아서 a~g 불을 켜줍니다.
    PORTC = data; 
}