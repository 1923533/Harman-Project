/*
 * 작성자: 김희정 
 * date: 2023.09.05
 * 내용: 외부 Interrupt를 이용한 간이 시계 실험
 */
 
#include <mega128.h>   
#include <delay.h>

typedef unsigned char U8;

const U8 seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

U8 N1, N10, N100, N1000;
U8 pos = 0;
U8 hour = 12, min = 0, sec = 0;

void Time_out(void); // 시간 표시 함수

void main(void){
  unsigned char i;

  DDRB = 0xF0;
  DDRD = 0xF0;
  DDRG = 0x0F;
  EIMSK = 0b00110000; // 외부 인터럽트 4,5 enable
  EICRB = 0b00001010; // 외부 인터럽트 4,5 falling edge
  
  SREG = 0x80;  // 전역 인터럽트 enable bit set
  
  while(1) {
    for(i = 0;i < 49;i++)
    Time_out(); // 약 1 초간 반복 표시
} // end of for

void Time_out(void))  // display
{
  PORTG = 0b00001000;  // 7 Seg DIG4 ON(PG3=1), 분 1 자리 표시
  PORTD = ((seg_pat[min % 10] & 0x0F) << 4) | (PORTD & 0x0F);
  PORTB = (seg_pat[min % 10] & 0x70) | (PORTB & 0x0F);
  delay_ms(5);
  
  PORTG = 0b00000100; // 7 Seg DIG3 ON(PG2=1), 분 10 자리 표시
  PORTD = ((seg_pat[min / 10] & 0x0F) << 4) | (PORTD & 0x0F);
  PORTB = (seg_pat[min / 10] & 0x70) | (PORTB & 0x0F);
  delay_ms(5);
  
  PORTG = 0b00000010; // 7 Seg DIG2 ON(PG1=1), 시간 1 자리 표시
  PORTD = ((seg_pat[hour % 10] & 0x0F) << 4) | (PORTD & 0x0F);
  PORTB = (seg_pat[hour % 10] & 0x70) | (PORTB & 0x0F);
  delay_ms(5);
  
  PORTG = 0b00000001; // 7 Seg DIG1 ON(PG0=1), 시간 10 자리 표시
  PORTD = ((seg_pat[hour / 10] & 0x0F) << 4) | (PORTD & 0x0F);
  PORTB = (seg_pat[hour / 10] & 0x70) | (PORTB & 0x0F);
  delay_ms(5);
}

interrupt[EXT_INT4] void external_int4(void)
{
  N1 = min % 10;      // 현재 분 1 자리 추출
  N10 = min / 10;     // 현재 분 10 자리 추출
  N100 = hour % 10;   // 현재 시간 1 자리 추출
  N1000 = hour / 10;  // 현재 시간 10 자리 추출

  if(pos == 0) N1 = (N1 + 1) % 10; // 현재 분 1 단위 표시 1 값
  else if(pos == 1) N10 = (N10 + 1) % 6; // 현재 분 10 단위 표시 +1 값 (60분)
  else if(pos == 2) {  // 시간 1 단위 1
       if(N1000 == 2 ) N100 = (N100 + 1) % 4 ; // 24 시간의 앞자리 2 이므로 20,21,22,23,24
       else N100 = (N100 + 1) % 10 ; // 0,1,2,3, 9,10 11,12,--,18, 19
  }
  else {  // pos=3 세 번 눌러짐
     if(N100 < 4) N1000 = (N1000 + 1) % 3 ; // N100<4 은 04 (00,01,02,03), (10,11,12,13), (20,21,22,23) => 10의 시간 자리는 0,1,2
     //  else if(N1000 != 1) N1000 = (N1000 + 1) % 3 ;
     else N1000 = (N1000 + 1) % 2;
  }

  hour = N1000 * 10 + N100; // 시간 계산 시간 세팅 값을 void Time_out ( 로
  min = N10 * 10 + N1;  // 분 계산
}

interrupt[EXT_INT5] void external_int5 (void )
{
   pos = (pos + 1) % 4;  // 입력 자리 이동
}