#include <mega128.h>

  char string[] = "This is USART0 control program. ^_______^"; // 전송 문자열
  char *pStr ; // 문자열 포인터
  unsigned char ch;
  
void Putch (char data); // 한 바이트 송신

void main(void)
{
  UCSR0A=0;
  UCSR0B = 0b10011000; // RXCIE0 [7] =1 : 수신 완료 인터럽트 enable
                       //송수신 enable TXEN0 [3] = 1, RXEN0 [4] =1
  UCSR0C = 0b00000110; // 비동기 데이터 8 비트 모드
  UBRR0H = 0;  // X TAL = 16MHz 일때 , BAUD = 9600
  UBRR0L = 103;
  
  SREG |= 0x80 ; // 전역 인터럽트 인에이블 I 비트 셋
  
  while(1);  // 문자 수신할 때까지 대기
}
interrupt [USART0_RXC] void usart0_rx (void))  // 수신 완료 인터럽트
{
  ch = UDR0; // 수신
  
  if(ch >= 'a' && ch <= 'z') ch = ch --'a' + 'A'; // 소문자 --> 대문자
  else if(ch >= 'A' && ch <= 'Z') ch = ch --'A' + 'a'; // 대문자 --> 소문자
  
  while((UCSR0A & 0x20) == 0x0); // UERE0=1 될 때까지 대기
  UDR0 = ch ; // 송신
  
  while((UCSR0A & 0x20) == 0x0); // UERE0=1 될 때까지 대기
  UDR0 = 0x0D ; // 송신
  
  pStr= string;
  while(*pStr != 0) Putch(*pStr++); // 문자열 전송
}

void Putch (char data)// 한 바이트 송신
{
  while((UCSR0A & 0x20) == 0x0);  //UDRE0[5] = 1 송신준비완료 될 때까지 대기
  UDR0 = data;  // 데이터 전송
}