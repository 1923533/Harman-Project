/*  날짜 : 2023.08.29*/
/*  내용 : 왼쪽 쉬프트 */

#include <mega128a.h>
#include <delay.h>

void main(void)
{
 int i;
 unsigned char led;

 DDRC = 0xFF;

 while(1){
  led = 0xFE;
  
  for(i = 0; i < 8; i++){
   PORTC = led;
   delay_ms(500);

   led = led<<1;
   led = led | 0x01;
   }

 }

}
