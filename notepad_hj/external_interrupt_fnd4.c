/*
 * 작성자: 김희정 
 * date: 2023.09.05
 * 내용: 2 개의 외부 Interrupt 를 이용한 10 진 네 자리 입력 실험
 */
 
#include <mega128.h>   

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
    DDRC = 0xFF;                // 포트 C 출력 설정
 //   DDRE = 0b00000010;      // 포트 E 입력(PE1 출력)
     DDRB = 0xF0;                // 포트 B 상위 4비트 출력 설정
    DDRD = 0xF0;                // 포트 D 상위 4비트 출력 설정
    DDRG = 0x0F;                // 포트 G 하위 4비트 출력 설정

                  
    PORTC = led;                 // 포트 C에 초기값 출력
    PORTG = 0b00001111;         // 맨 우측 7-Segment DIG4 ON(PG3=1)
	
    EIMSK = 0b11110000;     // 외부 인터럽트4 enable
    EICRB = 0b10111110;      // 외부 인터럽트4 falling edge 
    SREG |= 0x80;                 // 전역 인터럽트 enable set

    while(1){
        PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);   // A, B, C, D  표시
        PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F); 	 // E, F, G 표시

	
		if(EINT4_FLAG==0x01) {             
			PORTC = 0xFF;                 // 1이 되면 LED를 다 끄겠다포트 출력
			N1 = (N1 + 1) % 10;         // 값 +1

			EINT4_FLAG=0x00;
		}
		if(EINT5_FLAG==0x01) {
			led = led << 1;	        // 1비트 쉬프트
			led = led | 0b00000001;  // LSB bit set
			if(led == 0xFF) led = 0xFE;	 
			PORTC = led;                 // 포트 출력
			EINT5_FLAG=0x00;
		}
		if(EINT6_FLAG==0x01) {
			PORTC = 0xFF;                 // 포트 출력
			EINT6_FLAG=0x00;
			PORTC = 0xF0;                 // 포트 출력
		}
		if(EINT7_FLAG==0x01) {
			PORTC = 0xFF;                 // 포트 출력
			
			EINT7_FLAG=0x00;
  		   PORTC = 0x0F;                 // 포트 출력
		}		
	}

}

interrupt [EXT_INT4] void external_int4(void) 
{   
   SREG &= 0x7F;               // All Interrupt disable

   EINT4_FLAG=0x01;	

   SREG |= 0x80;          // All Interrupt enable
}

interrupt [EXT_INT5] void external_int5(void) 
{   
   SREG &= 0x7F;               // All Interrupt disable

   EINT5_FLAG=0x01;

   SREG |= 0x80;          // All Interrupt enable
}

interrupt [EXT_INT6] void external_int6(void) 
{   
   SREG &= 0x7F;               // All Interrupt disable

   EINT6_FLAG=0x01;	

   SREG |= 0x80;          // All Interrupt enable
}

interrupt [EXT_INT7] void external_int7(void) 
{   
   SREG &= 0x7F;               // All Interrupt disable

   EINT7_FLAG=0x01;	

   SREG |= 0x80;          // All Interrupt enable
}

