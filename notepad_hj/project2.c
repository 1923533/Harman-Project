/* 작성자 : 김희정  
 * 작성 시기 : 2023.09.11 
 * 수정     
 * 수정 key  
 */

#include <mega128a.h>          // 라이브러리 헤더 파일 포함
#include <delay.h>

typedef unsigned char U8;     // typedef 전처리문을 사용  
// typedef unsigned short U16;   // 변수의 다양한 사용을 위해 main문 밖 Global variable로 선언
// typedef unsigned int U32;

// U8 led = 0xFE;
// U8 N1 = 0, N10 = 0, N100 = 0, N1000 = 0; // 0 : 1 자리 , 1 : 10 자리 , 2 : 100 자리 , 3 : 1000 자리
const U8 seg_pat [10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,0x7f, 0x6f};
U8 class[8]={2,0,0,0,0,4,2,9};

U8 TIMER1_CTC_FLAG=0;
U8 TIMER2_OVF_FLAG=0;
U16 TIMER2_OVF_COUNT=0;

U8 EINT4_FLAG = 0x00;       // 변수 설정
U8 EINT5_FLAG = 0x00;
U8 EINT6_FLAG = 0x00;
U8 EINT7_FLAG = 0x00;

int clas_o;  // 학번 출력 변수 설정

void class_out(n1,n2,n3,n4);

// void Seg4_out(U8 N1, U8 N10, U8 N100, U8 N1000);    // 네 자리수 7 Segment 출력
// void Seg4_out2(U8 N1, U8 N10, U8 N100, U8 N1000);    

void main(void)
{                 
    int n1=0,n2=1,n3=2,n4=3;  //학번 출력 변수 위치 설정
	
	// 포트 초기화
    DDRC = 0xFF;            // 작동 확인용으로 LED 출력 설정
    // DDRE = 0b00001111;   // 포트 E 입력(PE1 출력)
    
    DDRB = 0xF0;     // 학번 디스플레이를 위한 포트 B 4,5,6,7 출력 설정 (LED)
    DDRD = 0xF0;     // 학번 디스플레이를 위한 포트 D 4,5,6,7 출력 설정 (LED)
    DDRG = 0x0F;     // 학번 디스플레이를 위한 포트 G 0,1,2,3 출력 설정 (COM) 
                  
    PORTC = 0xFF;           // 포트 C 출력 설정 
    PORTG = 0b00001111;    // 7 Segment ON (0x0F)

    EIMSK = 0b11110000;    // 인터럽트 사용 여부 결정 레지스터로 INT 4,5,6,7 enable
    EICRB = 0b10111011;    // INT 4,5,6,7은 B에 의해 Interrupt trigger 방식 설정 (외부 인터럽트 4 rising edge, 인터럽트 5 falling edge, 인터럽트 6 rising edge, 인터럽트 7 falling edge 설정 )
    SREG |= 0x80;          // status resister bit7(MSB)만 1으로 설정하여 모든 인터럽트를 허용(global interrupt enable),OR을 사용하여 나머지비트는 그대로
     
    for(class_o=0; class_o<4; class_o++)
	{
	  class_out(n1+class_o, n2+class_o, n3+class_o, n4+class_o);
	}	
	 
    // Seg4_out(0,0,0,2);      // 앞 4자리    
    // delay_ms(10);    

    while(1)   // 하위 4개 비트 출력
	{
	    class_out(n1+class_o, n2+class_o, n3+class_o, n4+class_o);

        // Seg4_out2(9,2,4,0);     // 뒤 4자리

        if(EINT4_FLAG==0x01) {            //인터럽트가 발생하면 
		    PORTC=0xFE;
            EINT4_FLAG=0;                //0으로 다시 바꿔주고
            EXIT4_TIMER2();
        } //end of if
		
        if(EINT5_FLAG==0x01) { 
		    PORTC=0xFB;
            EINT5_FLAG=0;
            EXIT5_TIMER2();
        } //end of if
		
        if(EINT6_FLAG==0x01) { 
             PORTC=0xEF;
			EINT6_FLAG=0;
            //EXIT6에서 PWM 1 함수를 실행.
        } //end of if
		 
        if(EINT7_FLAG==0x01) { 
            PORTC=0xBE;
			EINT7_FLAG=0;
            //EXIT7에서 PWM 2 함수를 실행.
        } //end of if 		
	}  //end of while
}  //end of main

void class_out(int n1, int n2, int n3, int n4)
{
   int i;
	
   for(i = 0;i <80 ;i++)  // 80번이 될때까지 디스플레이	
   {          
      PORTG = 0b00000001;                                         // 7-Segment DIG4(PG3=1) ON
      PORTD = ((seg_pat[class[n1]] & 0x0F) << 4) | (PORTD & 0x0F);       // [0]표시를 위해 ABCD표시, PORTD 하위 4bits 변경되지 않게 | 
      PORTB = (seg_pat[class[n1]] & 0x70) | (PORTB & 0x0F);             // [0]표시를 위해 EFG표시, PORTB 하위 4bits 변경되지 않게 | 
      delay_ms(2);          	                                   //구별을 위한 딜레이
	   
      PORTG = 0b00000010;                                         // 7-Segment DIG3(PG2=1) ON
      PORTD = ((seg_pat[class[n2]] & 0x0F) << 4) | (PORTD & 0x0F); 
      PORTB = (seg_pat[class[n2]] & 0x70) | (PORTB & 0x0F); 
      delay_ms(2); 
	   
      PORTG = 0b00000100;                                         // 7-Segment DIG2(PG1=1) ON
      PORTD = ((seg_pat[class[n3]] & 0x0F) << 4) | (PORTD & 0x0F);     
      PORTB = (seg_pat[class[n3]] & 0x70) | (PORTB & 0x0F); 
      delay_ms(2);
	   
      PORTG = 0b00001000;                                         // 7-Segment DIG1(PG0=1) ON
      PORTD = ((seg_pat[class[n4]] & 0x0F) << 4) | (PORTD & 0x0F);    
      PORTB = (seg_pat[class[n4]] & 0x70) | (PORTB & 0x0F); 
      delay_ms(2); 
   }  //end of for
} // end for class_out	
	
/* void Seg4_out(U8 N1, U8 N10, U8 N100, U8 N1000)   //학번 앞자리 2000 디스플레이
{
   U8 i;
	
   for(i = 0;i <40 ;i++)  // 40번이 될떄까지 디스플레이	
   {          
      PORTG = 0b00001000;                                         // 맨 우측 7Segment DIG4(PG3=1)ON, 1자리 표시
      PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);       // [0]표시를 위해 ABCD표시, PORTD 하위 4bits 변경되지 않게 | 
      PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F);             // [0]표시를 위해 EFG표시, PORTB 하위 4bits 변경되지 않게 | 
      delay_ms(2);          	                                   //구별을 위한 딜레이
	   
      PORTG = 0b00000100;                                         // 우측에서 두번째 7Segment DIG3(PG2=1)ON, 10자리 표시
      PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F); 
      PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F); 
      delay_ms(2); 
	   
      PORTG = 0b00000010;                                         // 우측에서 세번째 7Segment DIG2(PG1=1)ON, 100자리 표시
      PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);     
      PORTB = (seg_pat[N100] & 0x70 ) | (PORTB & 0x0F); 
      delay_ms(2);
	   
      PORTG = 0b00000001;                                         // 우측에서 네번째 7Segment DIG1(PG0=1)ON, 1000자리 표시
      PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);    
      PORTB = (seg_pat[N1000] & 0x70 ) | (PORTB & 0x0F); 
      delay_ms(2); 
   }  //end of for
}  //end of Seg4_out


void Seg4_out2(U8 N1, U8 N10, U8 N100, U8 N1000)   //학번 뒷자리 2251 디스플레이
{ 
     PORTG = 0b00001000; 
     PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F); 
     PORTB = (seg_pat[N1] & 0x70) | (PORTB & 0x0F);
     delay_ms(1);        	 //구별을 위한 최소한의 미세한 딜레이 
	 
     PORTG = 0b00000100; 
     PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F); 
     PORTB = (seg_pat[N10] & 0x70) | (PORTB & 0x0F); 
     delay_ms(1); 
	 
     PORTG = 0b00000010; 
     PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F); 
     PORTB = (seg_pat[N100] & 0x70) | (PORTB & 0x0F); 
     delay_ms(1);
		
     PORTG = 0b00000001; 
     PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F); 
     PORTB = (seg_pat[N1000] & 0x70) | (PORTB & 0x0F); 
     delay_ms(1);    
} //end of Seg4_out2
*/

interrupt [EXT_INT4] void external_int4(void){   
    SREG &= 0x7F;                                
    EINT4_FLAG=0x01;                            
    SREG |= 0x80;                                
}                                                      
 
interrupt [EXT_INT5] void external_int5(void){ 
    SREG &= 0x7F;
    EINT5_FLAG=0x01;     
    SREG |= 0x80;  
} 
                                
interrupt [EXT_INT6] void external_int6(void){ 
    SREG &= 0x7F;  
    EINT6_FLAG=0x01;  
    SREG |= 0x80;   
}

interrupt [EXT_INT7] void external_int7(void){ 
    SREG &= 0x7F;  
    EINT7_FLAG=0x01;       
    SREG |= 0x80;       
}



[Project 2]

1. while(1)문 시작 전에 7segments에 자신의 학번(8자리)을 쓰고 눈으로 확인할 수 있도록
   디스플레이 하기. 이때 학번 8자리 반드시 표시하고(표시방법은 자유롭게 프로그램)
   그리고 while(1)문에서 계속 학번 뒤 4자리를 계속 디스플레이 하기
   
2. while(1)이전에서 External INT 4, 5, 6, 7를 활성화(Enable 시킬 것)시키고, nesting은
   허용하지 않도록 함. while(1)에서는 아래의 이벤트를 수행한다
   
(1) External INT 4(rising edge) 이벤트가 발생하면
    8bit timer2 overflow mode로 학번 뒤 (2자리 x msec) 주기를 만들고,
    (예를 들어20211234라면 34msec)
    8bit timer2 overflow Interrupt를 발생시키고, 이를 이용하여 4500msec 될 때마다
    ADC를 통해 온도(free running mode) 측정하여 7-segment에
    디스플레이 할 것. 이때 학번 표시는 멈추고, 온도를 디스플레이 한다.
	   
    온도는 소수점 1의 자리까지, 소수점 표시 => 예 31.2 : 7-segments 4개 모두 사용)
    그리고 ADC를 통해 온도검출후 온도표시는 timer2 overflow interrupt 10회 동안만
    동작시킨다. 그이후 다시 학번을 정상적으로 표시한다.
	   
(2) External INT 5(falling edge) 발생하면 발생하면
    16bit timer1 compare match(CTC)로 학번 뒤 (2자리 x 100msec) 주기를 만들고,
   (예를 들어 20211234라면 3400msec) timer1 CTC interrupt를 발생시키고
   Interrupt 발생하면 ADC를 통해 전압(single mode)을 측정하고
   측정된 값을 UART를 동작시켜 PC화면에 전압 값을 디스플레이 시킬 것
  
(3) External INT 6(rising edge) 발생하면
    timer3의 90bit PWM mode6를 동작시키고, OC3A(PE3) pin으로 DUTY비-학번_뒤자
    리_2개 (예를 들어 20211234라면 34% duty)를 PWM를 출력시킨다.
	
(4) External INT 7(falling edge) 발생하면
    문제3의 timer3의 9bit PWM mode 출력을 중지시키고
    timer3의 9bit Phase correct PWM mode2를 동작시키고, OC3A(PE3) pin으로
    DUTY비-학번_뒤자리_2개 (예를 들어 20211234라면 34% duty)를 PWM를 출력시킨
    다.
	
3. interrupt service routine은 짧게 프로그래밍할 것
   변수는 typedef 문을 사용하여 선언할 것
   프로그램 맨 위에는 주석 작성