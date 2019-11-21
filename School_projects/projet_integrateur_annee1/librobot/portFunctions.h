#ifndef PORTFUNCTIONS_H
#define PORTFUNCTIONS_H
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 8000000U
#endif
#include <util/delay.h>
#include <avr/interrupt.h>
#include "can.h"

//The function names are based on the Arduino Library's

enum PORT { A, B, C, D };
enum MODE {INPUT, OUTPUT};
enum VALUE {OFF, ON};



void pinMode(uint8_t port, uint8_t pin, bool mode);

void digitalWrite(uint8_t port, uint8_t pin, bool value);

uint8_t readValue(uint8_t port, uint8_t pin);

uint8_t digitalRead(uint8_t port, uint8_t pin);


#endif