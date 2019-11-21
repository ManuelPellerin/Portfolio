
#ifndef DEBOUNCE_H
#define DEBOUNCE_H
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 8000000U
#endif
#include <util/delay.h>
#include "portFunctions.h"


bool debounceCheck(uint8_t port, uint8_t pin, bool buttonReversed = 0);

#endif