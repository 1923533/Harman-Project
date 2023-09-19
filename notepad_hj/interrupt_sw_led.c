/*
 * 작성자: 김희정 
 * date: 2023.09.05
 * 내용: 외부 Interrupt4에 의한 스위치 입력 실험 (응용)
 */
 
#include <mega128a.h>   
#include <delay.h>

unsigned char led = 0xFE;

unsigned char EINT4_FLAG =0x00;
unsigned char EINT5_FLAG =0x00;
unsigned char EINT6_FLAG =0x00;
unsigned char EINT7_FLAG =0x00;

const char seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

unsigned char  N1 = 0;

void main(void)
{                 
    // 포트 초기화
    DDRB = 0xF0;                // 포트 B 상위 4비트 출력 설정
    DDRD = 0xF0;                // 포트 D 상위 4비트 출력 설정
    DDRG = 0x0F;                // 포트 G 하위 4비트 출력 설정
    
    PORTG = 0b00001000;         // 맨 우측 7-Segment DIG4 ON(PG3=1)
	
    EIMSK = 0b00010000;         // 외부 인터럽트4 enable
    EICRB = 0b00000010;         // 외부 인터럽트4 falling edge 
    SREG |= 0x80;               // 전역 인터럽트 enable set

    while(1){
	    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);   // A, B, C, D  표시
        PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F); 	    // E, F, G 표시	
	}
}

// 외부 인터럽트 4 서비스 루틴
interrupt [EXT_INT4] void external_int4(void)
{
  N1 = (N1 + 1) % 10;    // 값 +1
}

interrupt [EXT_INT5] void external_int5(void) 
{   
   SREG &= 0x7F;         // All Interrupt disable

   EINT5_FLAG=0x01;

   SREG |= 0x80;          // All Interrupt enable
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

