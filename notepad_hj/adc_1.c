#include <mega128.h>
#include <delay.h>

typedef unsigned char u_char;

flash u_char seg_pat [10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07 , 0x7f ,0x6f};

void AD_disp (int); // A/D 변환값 표시

void main(void)
{
  int ad_val;
  
  DDRB = 0xF0;
  DDRD = 0xF0;
  DDRG = 0x0F;
  
  ADMUX = 0x06;   // ADC6 단극성 입력 선택
  ADCSRA = 0x87;  // ADEN=1, 16MHz 256 분주 --> 125kHz
  delay_ms(5);
  
  while(1){
      ADCSRA = 0xC7;  // ADEN=1, ADSC = 1 변환 시작
      while((ADCSRA & 0x10) == 0); // ADIF=1 이 될 때까지
	   
      ad_val = (int)ADCL + ((int)ADCH << 8); // A/D 변환값 읽기
	   
      AD_disp(ad_val); // A/D 변환값 표시
    }
}

