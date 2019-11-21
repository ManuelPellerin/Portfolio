#include "Pwm.h"

	
Pwm::Pwm(uint8_t compareMatchValueA, uint8_t compareMatchValueB, uint8_t counter)
{
	

	switch(counter)
	{
		case CNT0:
			counter_ = CNT0;
			DDRB |= (1 << PB3);

			// operating in ctc mode
			TCCR0A |= (1 << COM0A0 | 1 << WGM01);
			// prescaler set to 8
			TCCR0B |= (1 << CS02);

			OCR0A = compareMatchValueA;
		break;

		case CNT1:  // PWM sur Pins PD5
			//top is 212 to get 38khz
			
			DDRD |= ( 1 << PD5);
			counter_ = CNT1;

			OCR1A = compareMatchValueA;//PWM on PD5

			// Fast PWM, top = ICR1, mode 14
			// prescaler set to 1
			TCCR1A |= (1 << WGM11 | 1 << COM1A1);

			TCCR1B |= (1 << WGM12 | 1 << WGM13 | 1 << CS10) ;

			TCCR1C = 0;

			ICR1 = 212; // for 38 khz

		break;

		case CNT2:
		counter_ = CNT2;
		 //TODO : set CNT2 registers to an appropriate value
		break;
		
	}
}

void Pwm::stopPwm(){
	switch(counter_){
	case CNT0:
		TCCR0A &= ~(1 << COM0A0);
		break;
	case CNT1:
		TCCR1A &= ~(1 << COM1A1);
		break;
	case CNT2:
		TCCR2A = 0x00; //TODO set appropriate bit to 0
		break;
	}
}

void Pwm::startPwm(){
	switch(counter_){
	case CNT0:
		TCCR0A |= (1 << COM0A0);
		break;
	case CNT1:
		TCCR1A |= (1 << COM1A1);
		break;
	case CNT2:
		TCCR2A = 0x00; //TODO set appropriate bit to 1
		break;
	}
}	

uint8_t Pwm::getMatchA() const
{
	if ( counter_ == CNT0)
		return OCR0A;
	if ( counter_ == CNT1)
		return OCR1A;
	if ( counter_ == CNT2)
		return OCR2A;
	return 0;
}
uint8_t Pwm::getMatchB() const
{
	if ( counter_ == CNT0)
		return OCR0B;
	if ( counter_ == CNT1)
		return OCR1B;
	if ( counter_ == CNT2)
		return OCR2B;
	return 0;
}
uint8_t Pwm::getCounter() const
{
	return counter_;
}

void Pwm::setMatchA( uint8_t match)
{ 
	if ( counter_ == CNT0)
		OCR0A = match;
	if ( counter_ == CNT1)
		OCR1A = match;
	if ( counter_ == CNT2)
		OCR2A = match;
}
void Pwm::setMatchB(uint8_t match)
{
	if ( counter_ == CNT0)
		OCR0B = match;
	if ( counter_ == CNT1)
		OCR1B = match;
	if ( counter_ == CNT2)
		OCR2B = match;
}


