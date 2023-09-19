/*  날짜 : 2023.08.29*/
/*  내용 : sw off - on 될때마다 led 순차 점멸*/

#include <mega128a.h>

void main(void)
{
  unsigned char old_sw, new_sw;
  unsigned char led = 0xFE;
  
  DDRC = 0xFF;  // 포트 C 출력 설정
  DDRE = 0x00;  // 포트 E 입력 설정 
  PORTC = led;  // 포트 C.0 = 0 led on 초기값 출력
  
  old_sw = PINE & 0b00010000;  // PE4 SW1 상태값 추출
  
  while(1)
  {
    new_sw = PINE & 0b00010000;
	
	if((old_sw != 0) && (new_sw == 0))  // OFF(1) -> ON(0) 되는 순간 체크
	{
	  led = (led << 1) | 0x01;  // 1비트 쉬프트, 0비트 1로 채움
	  
	  if(led == 0xFF)  // LED 모두 off 상태면 초기값 재설정
	   led = 0xFE;
	   
	   PORTC = led;
	}
	  old_sw = new_sw; // 이전 상태 <- 현재 상태
  }
  
}