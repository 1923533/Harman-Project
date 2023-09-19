#include <mega128.h>
#include <delay.h>

const unsigned char seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

void Seg4_out(int num);

void main(void)
{
  int num = 0;

  DDRB = 0xF0; //포트 B 상위 4 비트 출력 설정
  DDRD = 0xF0; //포트 D 상위 4 비트 출력 설정
  DDRG = 0x0F; //포트 G 하위 4 비트 출력 설정
  
  PORTB = 0x0; // E, F, G, DP off
  PORTD = 0x0; // A, B, C, Doff

 while(1){
  Seg4_out(num);
  num++;
  
  if(num >9999 ) 
   num = 0;
  }
}
void Seg4_out(int num ) // 네 자리 정수 출력
{
 int i, N1000, N100, N10, N1, buf;
 
 N1000 = num / 1000; // 1000자리 추출
 buf = num % 1000;
 
 N100 = buf / 100; // 100자리 추출
 buf = buf % 100;
 
 N10 = buf / 10; // 10자리 추출
 N1 = buf % 10; // 1자리 추출
 
 for(i = 0;i < 2;i++)
 {
  PORTG = 0b00001000; // 맨 우측 7 Segment DIG4 ON(PG3=1)
  PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F); // A, B, C, D 표시
  PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F); // E, F, G 표시
  delay_ms(5);
  
  PORTG = 0b00000100; // 7 Segment DIG3 ON(PG2=1)
  PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F); // A, B, C, D 표시
  PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F); // E, F, G 표시
  delay_ms(5);

  PORTG = 0b00000010; // 7 Segment DIG2 ON(PG1=1)
  PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F); // A, B, C, D 표시
  PORTB = (seg_pat[N100] & 0x70 ) | (PORTB & 0x0F); // E, F, G 표시
  delay_ms(5);

  PORTG = 0b00000001; // 7 Segment DIG1 ON(PG0=1)
  PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F); // A, B, C, D 표시
  PORTB = (seg_pat[N1000] & 0x70 ) | (PORTB & 0x0F); // E, F, G 표시
  delay_ms(5);
 }
}