/*
 * 작성자: 김희정 
 * date: 2023.09.06
 * 내용: 타이머/카운터 0 을 이용한 LED 제어 실습 [일반 모드]
 */
 
#include <mega128a.h> 

typedef unsigned char U8;

U8 led = 0xFE;

U8 EINT0_FLAG =0x00;    


void main(void)
{
  DDRC = 0xFE;  // 포트 C 출력으로 설정
  PORTC = led;  // 포트 C에 초기값 출력
  
  TIMSK = 0x01;  // TOIE0 = 1(overflow interrupt enable)
  TCCR0 = 0x07;  // 일반 모드, 1024 분주
  TCNT0 = 0x00;  // 타이머/카운터0 레지스터 초기값
  SREG |= 0x80;  // 전역 인터럽트 enable 비트 1 set
  
  while(1);
}

interrupt[TIM0_OVF]void timer_int0(void)
{
  TCNT0 = 0x0;       // 초기값 재설정(0인 경우 생략 가능)
  led = led << 1;    // 1비트 shift
  led = led | 0x01;  // 최하위 비트 set
  
  if(led == 0xFF) led = 0xFE;  // led 모두 off이면 초기값 재설정
  
  PORTC = led;  // 포트 출력
  
  SREG |= 0x80;
}

// flag 사용함.
#include <mega128a.h> 

typedef unsigned char U8;

U8 led = 0xFE;

U8 OTINT0_FLAG = 0x00;  // Timer0 interrupt overflow flag
U8 CTINT0_FLAG = 0x00;  // Timer0 interrupt CTC flag

void main(void)
{
  DDRC = 0xFE;  // 포트 C 출력으로 설정
  PORTC = led;  // 포트 C에 초기값 출력
  
  TIMSK = 0x02;  // OCIE0 = 1(출력 비교 interrupt enable)
  TCCR0 = 0x0F;  // WGM01=1, CTC 모드, 1024 분주
  OCR0 = 155;    // 출력 비교 레지스터값(9.98ms 주기)
  TCNT0 = 0x0;   // 타이머/카운터0 레지스터 초기값
  SREG |= 0x80;  // 전역 인터럽트 enable 비트 1 set  
   
   while(1)
  {
      if(OTINT0_FLAG == 0x01)
	  {
	    TCNT0 = 0x00;
		
		led = led << 1;    // 1비트 쉬프트
        led = led | 0x01;  // 최하위 비트 set
  
        if(led == 0xFF) led = 0xFE; // 모두 off 이면 초기값 재설정
  
        PORTC = led; // 포트 출력
		
		TCNT0 = 0x00;
	  }
	  
      if(CTINT0_FLAG == 0x01)
	  {
	    TCNT0 = 0x00;
		
		led = led << 1;    // 1비트 쉬프트
        led = led | 0x01;  // 최하위 비트 set
  
        if(led == 0xFF) led = 0xFE; // 모두 off 이면 초기값 재설정
  
        PORTC = led; // 포트 출력
		
	    TCNT0 = 0x00;
	  }
   }    
}

// 타이머 카운터 0 출력비교 (TCNT0 = OCRO 일때) 인터럽트 서비스 루틴
// 인터럽트 발생 주기 1/16us * 1024 분주 * (1 + 155) = 9.98ms

interrupt [TIM0_OVF] void timer_int0 (void)
{
   SREG &= 0x7F;
    
   TINTO0_FLAG = 0x01;
    
   SREG |= 0x80;
}

interrupt [TIM0_COMP] void timer_comp0 (void)
{
   SREG &= 0x7F;
    
   TINTC0_FLAG = 0x01;
    
   SREG |= 0x80;
}