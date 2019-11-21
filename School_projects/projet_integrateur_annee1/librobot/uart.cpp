#include "uart.h"

void uartInit()
{
  UBRR0H = 0;
  UBRR0L = 0xCF;
 
  //UCSR0A |= 0;
  //Transmitter enable
  UCSR0A = 0;
  UCSR0B |= (1 << TXEN0 | 1 << RXEN0);
  UCSR0C |= (1 << UCSZ00 | 1 << UCSZ10);
}

void uartPrint ( uint8_t donnee ) {
  while(!(UCSR0A & (1 << UDRE0))){

  }
  UDR0 = donnee;
}


void uartPrint(char mots[41]){

    uint8_t i;

    for(i = 0; i < 40; i++) {
      uartPrint(mots[i]); 
    }  
}


void uartReception(uint8_t& data, bool& dataReceived){

  uint16_t timeoutMs = 2000;

  dataReceived = false;
  while(!(UCSR0A & (1 << RXC0)) && (timeoutMs > 0x00)){
    timeoutMs--;
    _delay_ms(1);
  }
  if (UCSR0A & (1 << RXC0)) {
    dataReceived = true;
    data = UDR0;
  }
  else {
    dataReceived = false;
    data = 0x00;
  }
  char msg2[41] = "finished_uart\n";
  DEBUG_PRINT((msg2));
}