#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// [수정 1] volatile 키워드 추가
volatile int count = 9;
volatile int state = 1;
volatile int ovr_count = 0;

ISR(INT1_vect)
{
    cli();
    _delay_ms(30);
    
    // 버튼을 눌렀는지 확실히 확인 (풀업 상태이므로 누르면 Low)
    if (!(PIND & (1 << PIND1))) 
    {
        state = 1;
        count = 9;
        ovr_count = 0;
        PORTC = (PORTC & 0xF0) | 9;
        PORTB &= ~(1 << PB0);
    }

    sei();
    EIFR &= ~(1 << INTF1);
}

ISR(TIMER2_OVF_vect)
{
    TCNT2 = 156;
    if (state == 1)
    {
        ovr_count++;
        // [테스트] 속도가 너무 느린지 확인하려면 20000을 2000으로 줄여보세요.
        if (ovr_count >= 20000) 
        {
            ovr_count = 0;
            count--;

            if (count < 0)
            {
                state = 0;
                count = 0;
            }
            // 카운트가 변경될 때만 디스플레이 업데이트
            PORTC = (PORTC & 0xF0) | count;
        }
    }
    else
    {
        ovr_count++;
        if (ovr_count >= 2000)
        {
            ovr_count = 0;
            PORTB ^= (1 << PB0);
        }
    }
}

void Init_Timer2(void)
{
    TCCR2 = 1 << CS21;
    TIMSK |= (1 << TOIE2);
    TCNT2 = 156;
}

int main(void)
{
    SREG |= 0x80;
    Init_Timer2();

    DDRC |= 0x0F;        // 7-Segment
    DDRB |= (1 << DDB0); // LED

    // [수정 2] 입력 설정 및 풀업 저항 활성화
    DDRD &= ~(1 << DDD1); // PD1 입력
    PORTD |= (1 << PD1);  // PD1 내부 풀업 활성화 (매우 중요)

    EIMSK |= (1 << INT1);
    EICRA |= (1 << ISC11);

    sei();

    while (1)
    {
    }
}