#include "portFunctions.h"
#include "debug.h"

void pinMode(uint8_t port, uint8_t pin, bool mode)
{

	if (mode == INPUT)
		switch (port) // sets the selected pin of the selected port to input
	{
	case A: DDRA &= ~(pin);
		break;
	case B: DDRB &= ~(pin);
		break;
	case C: DDRC &= ~(pin);
		break;
	case D: DDRD &= ~(pin);
	}
		
	else if (mode == OUTPUT)
		switch (port) // sets the selected pin of the selected port to output
	{
	case A: DDRA |= (pin);
		break;
	case B: DDRB |= (pin);
		break;
	case C: DDRC |= (pin);
		break;
	case D: DDRD |= (pin);
	}	
}

void digitalWrite(uint8_t port, uint8_t pin, bool value)
{
	pinMode(port, pin, OUTPUT);
	if (value == OFF)
		switch (port) // sets the selected pin of the selected port to 0
	{
	case A: PORTA &= ~(pin);
		break;
	case B: PORTB &= ~(pin);
		break;
	case C: PORTC &= ~(pin);
		break;
	case D: PORTD &= ~(pin);
	}
	else if (value == ON)
		switch (port) // sets the selected pin of the selected port to 1
	{
	case A: PORTA |= (pin);
		break;
	case B: PORTB |= (pin);
		break;
	case C: PORTC |= (pin);
		break;
	case D: PORTD |= (pin);
	}
}

uint8_t readValue(uint8_t port, uint8_t pin)
{
	pinMode(port, pin, OUTPUT);
	uint8_t readValue = 0;
	switch (port) // reads the value of the selected pin of the selected port
	{
	case A: readValue = ((PORTA & pin) == pin);
		break;
	case B: readValue = ((PORTB & pin) == pin);
		break;
	case C: readValue = ((PORTC & pin) == pin);
		break;
	case D: readValue = ((PORTD & pin) == pin);
	}
	return readValue;
}

uint8_t digitalRead(uint8_t port, uint8_t pin)
{
	pinMode(port, pin, INPUT);
	uint8_t portInput = PINA;
		switch (port) 
	{
	case A: portInput = PINA;
		break;
	case B: portInput = PINB;
		break;
	case C: portInput = PINC;
		break;
	case D: portInput = PIND;
	}
	if ((portInput & pin) == pin)
		return 1;
	return 0;
}
