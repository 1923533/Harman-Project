#include <mega128.h>
#include <delay.h>

void Putch (char); // 송신

void main(void)
{
  char string[] = "This is USART0 control program. ^_______^"; // 전송 문자열
  char *pStr ; // 문자열 포인터

  delay_ms(7000);  // PC 에서 메시지 확인 위해
  UCSR0A= 0x0 ;  // USART 초기화 (251p)
  UCSR0B = 0b00001000;  // 송신 인에이블 TXEN = 1 (253p)
  UCSR0C = 0b00000110;  // 비동기 [7], 데이터 8 비트 모드 (253,4p)
  UBRR0H = 0;  // X TAL = 16MHz 일때 , BAUD = 9600
  UBRR0L = 103;  // (257p)

  pStr= string;
  while(*pStr != 0) Putch(*pStr++); // 문자열 전송

  while(1);
}

void Putch (char data)// 한 바이트 송신
{
  while((UCSR0A & 0x20) == 0x0);  //UDRE0[5] = 1 송신준비완료 될 때까지 대기
  UDR0 = data;  // 데이터 전송
}