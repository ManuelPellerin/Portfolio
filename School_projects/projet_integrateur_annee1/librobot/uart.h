#ifndef UART_H
#define UART_H

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"

void uartInit();
void uartPrint(uint8_t donnee);
void uartPrint(char texte[41]);
void uartReception(uint8_t& data, bool& dataReceived);
    
#endif 