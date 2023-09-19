/*
 * 작성자: 김희정 
 * date: 2023.09.06
 * 내용: 10ms, 16ms 출력 비교
 */
 
#include <mega128a.h> 

typedef unsigned char U8;

U8 CNT_FLAG;
U8 OVF_FLAG;

U8 led = 0xFE;

void main(void)
{
  DDRC = 0xFF;    // 포트 C 출력으로 설정
  PORTC = led;    // 포트 C 에 초기값 출력
  
  TIMSK = 0x42;   // timer0 compare, timer2 overflow interrupt
  
  TCCR0 = 0x0F;   // WGM01=1, CTC 모드 , 1024 분주
  OCR0 = 155;     // 출력 비교 레지스터값 (9.98ms 주기)
  TCNT0 = 0x0;    // 타이머 카운터 0 레지스터 초기값
  
  TCCR2 = 0x05;   // overflow mode, 1024 분주
  TCNT2 = 0x0;    // 타이머/카운터 0 레지스터 초기값
  
  SREG |= 0x80;   // 전역 인터럽트 인에이블 비트 1 set

  while(1);
}

// 타이머/카운터 0 출력비교 (TCNT0 = OCRO 일때) 인터럽트 서비스 루틴
// 인터럽트 발생 주기 1/16us * 1024 분주 * (1 + 155) = 9.98ms

interrupt [TIM0_COMP] void timer_comp0 (void)
{
  if(CNT_FLAG==0)
  {
    PORTC &= 0xFE;
	CNT_FLAG = 1;
  }
  else 
  {
    PORTC |= 0x01;
	CNT_FLAG = 0;
  }
}

interrupt [TIM2_OVF] void timer_ovf2 (void)
{
  if(OVF_FLAG==0)
  {
    PORTC &= 0xAA;
	OVF_FLAG = 1;
  }
  else 
  {
    PORTC |= 0x10;
	OVF_FLAG = 0;
  }
}
