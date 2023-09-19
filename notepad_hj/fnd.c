/* 날짜 : 2023.08.29 */
/* 내용 : FND 불 켜기 */


#include <mega128.h>
#include <delay.h>

void main(void)
{
  unsigned char i,value;
  
  DDRB = 0xF0; //PORT B 4,5,6,7 비트 출력 설정
  DDRD = 0xF0; // PORT D 4,5,6,7 비트 출력 설정
  DDRG = 0x0F; // PORTG 0,1,2,3 비트 출력 설정
  PORTG = 0b0000 1 000; // 맨 우측 7 Segment DIG4 ON (PORTG3=1)
  PORTB = 0x00;0x00;// 초기값 PORTB 4,5,6,7 출력 0
  PORTD = 0x00;0x00;// 초기값 PORTD 4,5,6,7 출력 0

 while(1)
 {
   PORTD = 0b0001 0000;
   delay_ms(500);
   PORTD= 0b00000000;
  }
}
