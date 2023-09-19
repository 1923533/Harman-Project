/*  날짜 : 2023.08.29*/
/*  내용 : read sw */

#include <mega128a.h>

void main(void)
{
  unsigned char sw;
  
  DDRC = 0xFF;
  DDRE &= 0x0F;
  
  PORTC = 0xFF;
  
  while(1)
  {
    sw = PINE & 0b00010000;
	
	if(sw != 0) 
	 PORTC = 0xFF; 
	else 
	 PORTC = 0x00;
  }
}