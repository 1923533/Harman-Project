/*
 * 작성자: 김희정 
 * date: 2023.09.06
 * 내용: 타이머 카운터 0 을 이용한 LED 제어 실험(출력비교매치 인터럽트)
 */
 
#include <mega128a.h> 

typedef unsigned char U8;

U8 led = 0xFE;

void main(void)
{
  DDRC = 0xFF;   // 포트 C 출력으로 설정
  PORTC = led;   // 포트 C 에 초기값 출력
  
  TIMSK = 0x02;  // OCIE0 = 출력 비교 인터럽트 인에이블
  TCCR0 = 0x0F;  // WGM01=1, CTC 모드 , 1024 분주
  OCR0 = 155;    // 출력 비교 레지스터값 (9.98ms 주기
  TCNT0 = 0x0;   // 타이머 카운터 0 레지스터 초기값
  SREG = 0x80;   // 전역 인터럽트 인에이블 비트 I set

  while(1);
}

// 타이머 카운터 0 출력비교 (TCNT0 = OCRO 일때 ) 인터럽트 서비스 루틴
// 인터럽트 발생 주기 1/16us * 1024 분주 * (1 + 155) = 9.98ms

interrupt [TIM0_COMP] void timer_comp0 (void)
{
  led = led << 1;    // 1 비트 쉬프트
  led = led | 0x01;  // 최하위 비트 set
  
  if(led == 0xFF) led = 0xFE; // 모두 off 이면 초기값 재설정
  
  PORTC = led;  // 포트 출력
}