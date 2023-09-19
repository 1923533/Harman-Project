#include <mega128.h>
#include <delay.h>

const unsigned char seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

void main(void)
{
  unsigned char oldkey, key, num = 0;
  DDRB = 0xF0; // 포트 B 상위 4 비트 출력 설정
  DDRD = 0xF0; // 포트 D 상위 4 비트 출력 설정
  DDRG = 0x0F; // 포트 G 하위 4 비트 출력 설정 
  DDRE = 0x00; // 포트 E 입력설정
  
  PORTG = 0b00001000; // 맨 우측 7 Segment DIG4 ON(PG3=1)
  PORTB = 0x0;        // E, F, G, DP off
  PORTD = 0x0;        // A, B, C, D off
  
  oldkey = PINE & 0b00010000; // SW1 상태만 추출
while(1)
{
  PORTD = ((seg_pat[num] & 0x0F) << 4) | (PORTD & 0x0F);  // A, B, C, D 표시
  PORTB = (seg_pat[num] & 0x70) | (PORTB & 0x0
  F);         // E, F, G 표시
  
  key = PINE & 0b00010000;   // SW1 상태만 추출
  
  if(oldkey != 0 && key == 0) // 먼저 상태 OFF, 현 상태 ON ?
  {
    num = (num + 1) % 10;     // num 값 1 증가 9 다음은 0)
  }
  oldkey = key;   // 먼저 상태 현상태
 }
}