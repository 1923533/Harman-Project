/*
 * 작성자: 김희정 
 * date: 2023.09.05
 * 내용: External Interrupt 4 에 의한 LED 점멸 실험 (falling edge/rising edge)
 */

#include <mega128.h>   

unsigned char led = 0xFE;

unsigned char EINT4_FLAG =0x00;
unsigned char EINT5_FLAG =0x00;
unsigned char EINT6_FLAG =0x00;
unsigned char EINT7_FLAG =0x00;

void main(void)
{                 
    // 포트 초기화
	DDRC = 0xFF;                // 포트 C 출력 설정
	
	PORTC = led;                // 포트 C에 초기값 출력

    EIMSK = 0b11110000;         // 외부 인터럽트 all enable
    EICRB = 0b10101010;         // 외부 인터럽트 all falling edge 
    SREG |= 0x80;               // 전역 인터럽트 enable set

    while(1){
		if(EINT4_FLAG==0x01) {             
			PORTC = 0x00;               
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
			PORTC = 0xAA;                 // 포트 출력
			EINT6_FLAG=0x00;
		}
		if(EINT7_FLAG==0x01) {
			PORTC = 0x55;                 // 포트 출력
			EINT7_FLAG=0x00;
		}		
	}
}

interrupt [EXT_INT4] void external_int4(void) 
{   
   SREG &= 0x7F;             // All Interrupt disable

   EINT4_FLAG=0x01;	

   SREG |= 0x80;             // All Interrupt enable
}

interrupt [EXT_INT5] void external_int5(void) 
{   
   SREG &= 0x7F;             // All Interrupt disable

   EINT5_FLAG=0x01;

   SREG |= 0x80;            // All Interrupt enable
}

interrupt [EXT_INT6] void external_int6(void) 
{   
   SREG &= 0x7F;            // All Interrupt disable

   EINT6_FLAG=0x01;	

   SREG |= 0x80;           // All Interrupt enable
}

interrupt [EXT_INT7] void external_int7(void) 
{   
   SREG &= 0x7F;           // All Interrupt disable

   EINT7_FLAG=0x01;	

   SREG |= 0x80;          // All Interrupt enable
}

