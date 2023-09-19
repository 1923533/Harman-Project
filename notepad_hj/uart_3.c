#include <mega128.h>

void main(void)
{
  unsigned char character;

  UCSR0A=0x0;
  UCSR0B = 0b00011000; // 송수신부 enable TXEN0 [3] = 1, RXEN0 [4] =1
  UCSR0C = 0b00000110; //비동기 :0, No parity: 00, 정지 bit 1 이면 :0, 데이터 8 비트 모드
  UBRR0H = 0;// 16MHz 일때 , BAUD = 9600
  UBRR0L = 103
  
while(1)
{
  while((UCSR0A & 0x80) == 0x0); // RXC0=1 될 때까지 대기
                                 //수신 버퍼에 새로운 데이터가 수신되면 RXC0 = 1
  character= UDR0; // 수신 , 값을 읽으면 바로 RXC0 [7]bit = 0 됨
  
  if((character >= 'a') && character <= 'z'))
      character= character -'a' + 'A'; // 소문자 --> 대문자
  else if(( character >= 'A') && character <= 'Z'))
     character = character -'A' + 'a'; // 대문자 --> 소문자
 
  while((UCSR0A & 0x20) == 0x0); // 송신 버퍼가 비어지면 UDRE0 = 1 이 될 때까지 대기
                                 // 전송 준비가 되었는지 확인하는 것
  UDR0 = character; // 송신
 }
}