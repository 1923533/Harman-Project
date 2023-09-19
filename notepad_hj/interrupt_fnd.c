/*
 * interrupt_2.c
 * date: 2023.09.05
 * Author: hee jeong
 */

#include <mega128.h>   
#include <delay.h>

typedef unsigned char U8;

unsigned char led = 0xFE;   // led on

// 변수 만들고 초기값 0 설정 
unsigned char EINT4_FLAG =0x00;    
unsigned char EINT5_FLAG =0x00;
unsigned char EINT6_FLAG =0x00;
unsigned char EINT7_FLAG =0x00;

const U8 seg_pat [10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,0x7f, 0x6f};
U8 N1 = 0, N10 = 0, N100 = 0, N1000 = 0; // 0 : 1 자리 , 1 : 10 자리 , 2 : 100 자리 , 3 : 1000 자리
U8 pos = 0;

void Seg4_out(void); // 네 자리수 7 Segment 출력

void main(void)
{                 
    // 포트 초기화
    // DDRC = 0xFF;         // 포트 C 출력 설정
    // DDRE = 0b00000010;   // 포트 E 입력(PE1 출력)
    
    DDRB = 0xF0; // 포트 B 상위 4 비트 출력 설정
    DDRD = 0xF0; // 포트 D 상위 4 비트 출력 설정
    DDRG = 0x0F; // 포트 G 하위 4 비트 출력 설정, 각 fnd 선택하는 것
                  
    PORTC = led;            // 포트 C에 초기값 출력
    PORTG = 0b00001000;     // 맨 우측 7 Segment DIG4 ON(PG3=1)

    EIMSK = 0b11110000;     // 외부 인터럽트 all enable
    EICRB = 0b10111110;     // 외부 인터럽트4 falling edge, 인터럽트5 rising edge, 인터럽트6 rising edge, 인터럽트7 falling edge 설정 
    SREG |= 0x80;           // 전역 인터럽트 enable set
        
    while(1){ 
       Seg4_out();        
    }
}

void Seg4_out(void)
{
   PORTG = 0b00001000; // 7 Seg DIG4 ON(PG3=1), 1 자리 표시
   PORTD = ((seg_pat N1 ] & 0x0F) << 4 ) | (PORTD & 0x0F); // A, B, C, D 표시
   PORTB = (seg_pat N1 ] & 0x70 ) | (PORTB & 0x0F); // E, F, G 표시
   delay_ms(5);
   
   PORTG = 0b00000100; // 7 Seg DIG3 ON(PG2=1), 10 자리 표시
   PORTD = ((seg_pat N10 ] & 0x0F) << 4) | (PORTD & 0x0F); // A, B, C, D 표시
   PORTB = (seg_pat N10 ] & 0x70 ) | (PORTB & 0x0F); // E, F, G 표시
   delay_ms(5);

   PORTG = 0b00000010; // 7 Seg DIG2 ON(PG1=1), 100 자리 표시
   PORTD = ((seg_pat N100 ] & 0x0F) << 4) | (PORTD & 0x0F);
   PORTB = (seg_pat N100 ] & 0x70 )| (PORTB & 0x0F);
   delay_ms(5);

   PORTG = 0b00000001 ; // 7 Seg DIG1 ON(PG0=1), 1000 자리 표시
   PORTD = ((seg_pat N1000 ] & 0x0F) << 4) | (PORTD & 0x0F);
   PORTB = (seg_pat N1000 ] & 0x70 ) | (PORTB & 0x0F);
   delay_ms(5);
}

// 외부 인터럽트 4 서비스 루틴
interrupt [EXT_INT4] void external_int4 (void)
{
  if(pos == 0) N1 = (N1 + 1) % 10; // 1 자리 +1
  else if(pos == 1) N10 = (N10 + 1) % 10; // 10 자리 +1
  else if(pos == 2) N100 = (N100 + 1) % 10; // 100 자리 +1
  else N1000 = (N1000 + 1) % 10;  // 1000 자리 +1
}

// 외부 인터럽트 5 서비스 루틴
interrupt [EXT_INT5] void external_int5 (void)
{
  pos = (pos + 1) % 4;  // 입력 자리 이동
}

interrupt [EXT_INT6] void external_int6(void) 
{   
   SREG &= 0x7F;           // All Interrupt disable

   EINT6_FLAG=0x01;	

   SREG |= 0x80;           // All Interrupt enable
}

interrupt [EXT_INT7] void external_int7(void) 
{   
   SREG &= 0x7F;           // All Interrupt disable

   EINT7_FLAG=0x01;	

   SREG |= 0x80;           // All Interrupt enable
}

