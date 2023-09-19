/*
 * 작성자: 김희정 
 * date: 2023.09.06
 * 내용: 타이머 카운터 0 을 이용한 시계 제작
 */
 
#include <mega128a.h> 

typedef unsigned char U8;

flash U8 seg_pat [10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

U8 N1 = 0, N10 = 0, N100 = 0, N1000 = 0;
U8 pos = 0;    // 0 : 1 자리 , 1 : 10 자리 , 2 : 100 자리 , 3 : 1000 자리
U8 hour = 12, min = 0, sec = 0;
U8 cnt = 0;

void Time_out(void); // 시간 출력 함수

void main(void)
{
  DDRB = 0xF0; // 포트 B 상위 4 비트 출력 설정
  DDRD = 0xF0; // 포트 D 상위 4 비트 출력 설정
  DDRG = 0x0F; // 포트 G 하위 4 비트 출력 설정
  
  EIMSK = 0b00110000;  // 외부 인터럽트 4,5 enable
  EICRB = 0b00001010;   // 외부 인터럽트 4,5 : falling edge

  TIMSK = 0x01 ;  // TOIE0 = 1 타이머 카운터 0 오버플로우 인터럽트 enable
  TCCR0 = 0x07 ;  // 일반모드 , 1024 분 주
  TCNT0 = 0x00;   // 타이머 카운터 0 레지스터 초기값
  SREG |= 0x80;   // 전역 인터럽트 enable 비트 1 set
  
  while(1) 
  {
    Time_out(); // 시간 출력
	
    if(cnt >= 61) 
	{ // 시간 값 갱신
	  cnt = 0;
      sec = sec + 1;
	
     if(sec == 60) 
	 {
       sec = 0;
       min = min + 1;
	   
       if(min == 60) 
	   {
          min = 0;
          hour = (hour + 1) % 24;
       }
      }
    }
   }
}