/* 날짜 : 2023.09.18 */
/* 직상지 : 김희정     */
/* 내용 : project2  */
/* 수정             */
/* 수정 key         */

#include <mega128a.h>          // 라이브러리 헤더 파일 포함
#include <delay.h>

typedef unsigned char U8;  // typedef 전처리문을 사용  
typedef unsigned short U16;  // typedef 전처리문을 사용 
typedef unsigned int U32;     // typedef 전처리문을 사용 

const char seg_pat [10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,0x7f, 0x6f};
U8 class[8]={2,0,0,0,0,4,2,9};

U8 EINT4_FLAG = 0x00;       // 변수 설정
U8 EINT5_FLAG = 0x00;
U8 EINT6_FLAG = 0x00;
U8 EINT7_FLAG = 0x00;

U8 TIMER1_CTC_FLAG_BIT=0;
U8 TIMER2_OVF_FLAG_BIT=0;
U16 TIMER2_OVF_COUNT=0;

U8 ADC_RESULT_FLAG=0;
U32 TEMP_ADC_VALUE=0;

U8 NA100, NA10, NA1;
U8 NV100, NV10, NV1;
U32 ad_value=0;

int class_o;  // 학번 출력 변수 설정

void class_out(int n1,int n2,int n3,int n4);
void class_out2(int n1,int n2,int n3,int n4);

void EXIT4_TIMER2(void);
void EXIT5_TIMER1(void);
void EXIT6_PWM_M6(void);
void EXIT7_PWM_M2(void);

void UART_volt(void);
void AD_display(int value);
void SEG7_TEMP_DIS(int val);

void main(void)
{                 
    int n1=0,n2=1,n3=2,n4=3;  //학번 출력 변수 위치 설정
   
    DDRB = 0xF0;       
    DDRD = 0xF0;
    DDRE |= 0x08;       
    DDRG = 0x0F;
    DDRC = 0xFF;
              
    PORTC = 0xFF;
    PORTD = 0xFF;  

    EIMSK = 0b11110000;    // 인터럽트 사용 여부 결정 레지스터로 INT 4,5,6,7 enable
    EICRB = 0b10111011;    // INT 4,5,6,7은 B에 의해 Interrupt trigger 방식 설정 (외부 인터럽트 4 rising edge, 인터럽트 5 falling edge, 인터럽트 6 rising edge, 인터럽트 7 falling edge 설정 )
    SREG |= 0x80;          // status resister bit7(MSB)만 1으로 설정하여 모든 인터럽트를 허용(global interrupt enable),OR을 사용하여 나머지비트는 그대로
        
    for(class_o=0; class_o<4; class_o++){
        class_out(n1+class_o, n2+class_o, n3+class_o, n4+class_o);
    }   
    
    while(1){
        if(ADC_RESULT_FLAG==0) class_out2(n1+class_o, n2+class_o, n3+class_o, n4+class_o);     //온도 결과값이 없을 때는 학번을 계속 띄움 class_out2(n1+class_o, n2+class_o, n3+class_o, n4+class_o):같은 결과
        else SEG7_TEMP_DIS(TEMP_ADC_VALUE);            //온도 값이 나올때는 온도를 디스플레이함
      
        if(EINT4_FLAG==0x01) {            //인터럽트가 발생하면 
            PORTC=0xFE;
            EINT4_FLAG=0;                //0으로 다시 바꿔주고
            EXIT4_TIMER2();             //EXIT4_TIMER2함수를 실행한다.
        } //end of if
      
        if(EINT5_FLAG==0x01) { 
            PORTC=0xFB;
            EINT5_FLAG=0;
            EXIT5_TIMER1();
        } //end of if
      
        if(EINT6_FLAG==0x01) { 
            PORTC=0xEF;
            EINT6_FLAG=0;
            EXIT6_PWM_M6();
        } //end of if
       
        if(EINT7_FLAG==0x01) { 
            PORTC=0xBF;
         EINT7_FLAG=0;
            EXIT7_PWM_M2();
        } //end of if                       
        
        if(TIMER2_OVF_FLAG_BIT==1){                     //오버플로우 모드가 발생하면
            TIMER2_OVF_FLAG_BIT=0;                      //0으로 바꿔주고
            TIMER2_OVF_COUNT++;                         //1씩 더해간다           
            

            if(TIMER2_OVF_COUNT>=1552){                  //4.5sec(주기가 2.9msec이므로 1552번)  
            
                PORTG = 0b00001111;                    
                PORTD = 0x00;                                   
                PORTB = 0x00;                              
                delay_ms(100);  
                            
                TIMER2_OVF_COUNT=0;        
                ADC_RESULT_FLAG++;                      
                ADMUX = 7;                              // A/D 변환기 멀티플렉서 선택 레지스터 
                ADCSRA = 0xc2;                          // ADEN=1, ADSC=1, ADFR=1, 16MHz, 8분주  
                delay_us(500);                          // 변환시간이 길어져 발생하는 ADC오류의 방지를 위한 딜레이가 필요함 
                TEMP_ADC_VALUE = (int)ADCL + ((int)ADCH << 8);  //A/D 변환값 읽기         
                
                if(ADC_RESULT_FLAG==10){ 
                ADC_RESULT_FLAG %= 10;                  // 10회동안만 온도 표시    
               
                 // TIMSK &= 0b10111111;                   // TOIE2 = 0으로 설정하여 인터럽트가 안걸리게 하는 방법
                 TCCR2 &= 11111000; // 타이머 정지
                 
                } //end of if  
            } //end of if                  
        } //end of if                     
       
        if(TIMER1_CTC_FLAG_BIT==1){
            TIMER1_CTC_FLAG_BIT=0;  
            ADMUX = 6;                                  //ADC6 단극성 입력 선택
            ADCSRA = 0x82;                              
            ADCSRA = 0xC2;                              // ADEN=1, 8분주 변환 시작
          
            while((ADCSRA & 0x10) == 0);                // ADIF=1이 될 때까지(Single)
            ad_value = (int)ADCL + ((int)ADCH << 8);    // A/D 변환값 읽기
            AD_display(ad_value);                       // A/D 변환값 표시
            UART_volt();                                // 유아트 디스플레이를 위한 함수
        } //end of if 
    }  //end of while
}  //end of main

void class_out(int n1, int n2, int n3, int n4)
{
   int i;
    
   for(i = 0;i <160 ;i++)  // 80번이 될때까지 디스플레이    
   {          
      PORTG = 0b00000001;                                         // 7-Segment DIG4(PG3=1) ON
      PORTD = ((seg_pat[class[n1]] & 0x0F) << 4) | (PORTD & 0x0F);       // [0]표시를 위해 ABCD표시, PORTD 하위 4bits 변경되지 않게 | 
      PORTB = (seg_pat[class[n1]] & 0x70) | (PORTB & 0x0F);             // [0]표시를 위해 EFG표시, PORTB 하위 4bits 변경되지 않게 | 
      delay_ms(1);                                                 //구별을 위한 딜레이
       
      PORTG = 0b00000010;                                         // 7-Segment DIG3(PG2=1) ON
      PORTD = ((seg_pat[class[n2]] & 0x0F) << 4) | (PORTD & 0x0F); 
      PORTB = (seg_pat[class[n2]] & 0x70) | (PORTB & 0x0F); 
      delay_ms(1); 
       
      PORTG = 0b00000100;                                         // 7-Segment DIG2(PG1=1) ON
      PORTD = ((seg_pat[class[n3]] & 0x0F) << 4) | (PORTD & 0x0F);     
      PORTB = (seg_pat[class[n3]] & 0x70) | (PORTB & 0x0F); 
      delay_ms(1);
       
      PORTG = 0b00001000;                                         // 7-Segment DIG1(PG0=1) ON
      PORTD = ((seg_pat[class[n4]] & 0x0F) << 4) | (PORTD & 0x0F);    
      PORTB = (seg_pat[class[n4]] & 0x70) | (PORTB & 0x0F); 
      delay_ms(1); 
   }  //end of for
} // end for class_out   

void class_out2(int n1, int n2, int n3, int n4)
{           
      PORTG = 0b00000001;                                         // 7-Segment DIG4(PG3=1) ON
      PORTD = ((seg_pat[class[n1]] & 0x0F) << 4) | (PORTD & 0x0F);       // [0]표시를 위해 ABCD표시, PORTD 하위 4bits 변경되지 않게 | 
      PORTB = (seg_pat[class[n1]] & 0x70) | (PORTB & 0x0F);             // [0]표시를 위해 EFG표시, PORTB 하위 4bits 변경되지 않게 | 
      delay_us(100);                                                 //구별을 위한 딜레이
       
      PORTG = 0b00000010;                                         // 7-Segment DIG3(PG2=1) ON
      PORTD = ((seg_pat[class[n2]] & 0x0F) << 4) | (PORTD & 0x0F); 
      PORTB = (seg_pat[class[n2]] & 0x70) | (PORTB & 0x0F); 
      delay_us(100); 
       
      PORTG = 0b00000100;                                         // 7-Segment DIG2(PG1=1) ON
      PORTD = ((seg_pat[class[n3]] & 0x0F) << 4) | (PORTD & 0x0F);     
      PORTB = (seg_pat[class[n3]] & 0x70) | (PORTB & 0x0F); 
      delay_us(100);
       
      PORTG = 0b00001000;                                         // 7-Segment DIG1(PG0=1) ON
      PORTD = ((seg_pat[class[n4]] & 0x0F) << 4) | (PORTD & 0x0F);    
      PORTB = (seg_pat[class[n4]] & 0x70) | (PORTB & 0x0F); 
      delay_us(100); 
} // end for class_out 

interrupt [EXT_INT4] void external_int4(void){   
    SREG &= 0x7F;                                
    EINT4_FLAG=0x01;
    TCNT2 = 210;                             
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

interrupt [TIM2_OVF] void TIMER2_OVF_int(void){ 
    SREG &= 0x7F;  
    TIMER2_OVF_FLAG_BIT=1; 
    TCNT2 = 210;                                 //초기값을 인터럽트 안에도 써줘야함
    SREG |= 0x80;         
}

interrupt [TIM1_COMPA] void TIMER1_CTC_int(void){ 
    SREG &= 0x7F;   
    TIMER1_CTC_FLAG_BIT=1;        
    SREG |= 0x80;         
}

void EXIT4_TIMER2(void){            
    TIMSK |= 0b01000000;                        // overflow interrupt enable
    TCCR2 = 0b00000101;                         // normal모드, 프리스케일= 1024분주
    //TCNT2 = 210;                                // 210=2.9ms (학번뒷자리 29이라 8비트로 29ms불가)
} //end of EXIT4_TIMER2              
  
void EXIT5_TIMER1(void){
    TCCR1A = 0b00000000;                        // mode4
    TCCR1B = 0b00001101;                        // mode4(CTC), 프리스케일 = 1024분주  
    TCCR1C = 0x00;
          
    TCNT1H = 0x0;                               //레지스터 초기화
    TCNT1L = 0x0;                               //레지스터 초기화   
    OCR1AH= 0x1F;                               //7968,hex1F20로 나온 것.
    OCR1AL = 0x20;                              //510ms (5100ms 16비트로 불가하여 510ms로 설정)
    TIMSK |= 0x10;                              //compare match interrupt enable
} //end of EXIT5_TIMER1

void EXIT6_PWM_M6(void){           
    TCCR3A = 0b10000010;                        //OC3A(PE3)에 파형출력, mode6 
    TCCR3B = 0b00001100;                        //mode6(Fast PWM모드),256분주 - 122.07Hz 구동
    TCCR3C = 0x00;              
      
    TCNT3H = 0x0;                               //레지스터 초기화
    TCNT3L = 0x0;                               //레지스터 초기화    
    OCR3AH= 0x01;                               //260,hex104로 나온 것.
    OCR3AL = 0x04;                     
    
    DDRE |= 0x08;                               //TIMER3 PWM PIN PE3은 DDRE 3을 출력설정해야 PWM 파형이 출력됨
} //end of EXIT6_PWM_M6

void EXIT7_PWM_M2(void){
    TCCR3A = 0b10000010;                        //OC3A(PE3)에 파형출력, mode2 
    TCCR3B = 0b00000100;                        //mode2(PWM Phase correct 모드),256분주 - 61.035Hz
    TCCR3C = 0x00;              
      
    TCNT3H = 0x0;                               //레지스터 초기화
    TCNT3L = 0x0;                               //레지스터 초기화   
    OCR3AH= 0x01;                               //260,hex104로 나온 것.
    OCR3AL = 0x04; 
    DDRE |= 0x08;                               //PE3 출력 
} //end of EXIT7_PWM_M2

void SEG7_TEMP_DIS(int val){ 
    float fval;
    int ival, buf; 
         
    fval = (float)val * 5.0 / 1024.0;             // 온도값으로 변환
    ival = (int)(fval * 1000.0+0.5);                  // 반올림 후 정수화(소수 첫째자리까지)    
    
    NA100 = ival / 100;                           // 정수부 10의 자리 추출
    buf = ival % 100; 
    NA10 = buf / 10;                              // 한자리수 추출
    NA1 = buf % 10;                               // 소수 첫째 자리 추출 
    
    PORTG = 0b00001000;                           // PG3=1, 소수 첫째 자리
    PORTD = ((seg_pat[NA1] & 0x0F) << 4) | (PORTD & 0x0F);  //PORTD 하위 4bits 변경되지 않게 |
    PORTB = (seg_pat[NA1] & 0x70 ) | (PORTB & 0x0F);        //PORTB 하위 4bits 변경되지 않게 |
    delay_ms(1);      
    
    PORTG = 0b00000100;                           // PG2=1
    PORTD = 0x00;                                   
    PORTB = 0x80;                                 // DP on(소수점) 
    delay_ms(1);         
    
    PORTG = 0b00000010;                           // PG1=1, 정수 일의 자리      
    PORTD = ((seg_pat[NA10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[NA10] & 0x70 ) | (PORTB & 0x0F); 
    delay_ms(1);           
    
    PORTG = 0b00000001;                           // PG0=1,정수 10의 자리
    PORTD = ((seg_pat[NA100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[NA100] & 0x70 ) | (PORTB & 0x0F);                                                                                                                                           
    delay_ms(1); 
} //end of SEG7_TEMP_DIS

void AD_display(int value) { 
    float fvalue;
    int ivalue, buff; 

    fvalue = (float)value * 5.0 / 1024.0;        // 전압 값으로 변환
    ivalue = (int)(fvalue * 100.0 + 0.5);        // 반올림 후 정수화,(소수 둘째자리까지) 
    
    NV100 = ivalue / 100;                        // 정수부 추출
    buff = ivalue % 100; 
    
    NV10 = buff / 10;                            // 소수 첫째 자리 추출
    NV1 = buff % 10;                             // 소수 둘째 자리 추출
} //end of AD_display

void UART_volt(){    
    UCSR0A = 0x0;               // USART 초기화 
    UCSR0B = 0b00001000;        // 송신 인에이블 TXEN = 1 
    UCSR0C = 0b00000110;        // 비동기[7], 데이터 8비트 모드 
    UBRR0H = 0;                 // X-TAL = 16MHz 일때, BAUD = 9600
    UBRR0L = 103;  
                                        
    while((UCSR0A & 0x20) == 0x0);     //송신 버퍼가 비어지면 UDRE0[5]= 1이 될 때까지 대기       
     UDR0 =NV100+0x30;                 //위 추출 숫자 아스키코드로 데이터 전송    
     
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 ='.';                        //문자'.'전송 
     
    while((UCSR0A & 0x20) == 0x0);                           
     UDR0 =NV10+0x30;                  //위 추출 숫자 아스키코드로 데이터 전송  
        
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =NV1+0x30;                   //위 추출 숫자 아스키코드로 데이터 전송
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 ='V';                        //문자'V'전송
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =' ';                        //알아보기 쉽게 띄어쓰기
} //end of UART_volt