#include "Pwm.h"
#include "debug.h"

	
Pwm::Pwm(uint8_t compareMatchValueA, uint8_t compareMatchValueB, uint8_t counter)
{
	

	switch(counter)
	{
		//used for dem memes
		case CNT0:
			counter_ = CNT0;
			DDRB |= (1 << PB3);

			// operating in ctc mode
			TCCR0A |= (1 << COM0A0 | 1 << WGM01);
			// prescaler set to 8
			TCCR0B |= (1 << CS02);

			OCR0A = compareMatchValueA;
		break;

		//used for motors
		case CNT1:  // PWM sur Pins PD4 et PD5
			
			DDRD |= (1 << PD5 | 1 << PD4 );
			counter_ = CNT1;
			// value between 0 and 255 inclusively
			OCR1B = compareMatchValueB; //PWM on PD4
			OCR1A = compareMatchValueA;//PWM on PD5

			//Phase correct, 8-bit PWM.
			// clock divided by 8 - implies fixed Pwm frequency
			TCCR1A |= 1 << COM1A1 | 1 << COM1B1 | 1 << WGM10 ;

			TCCR1B |= 1 << CS11 ;

			TCCR1C = 0;
		break;

		//used for ir reception
		case CNT2:
			counter_ = CNT2;

			//operating in ctc mode
			TCCR2A |= (1 << WGM21);

			//TODO WHEN YOU WANT TO USE
			//THIS COUNTER

			//Set the prescaler to
			//TCCR2B = (1 << CS20); (ir, 1)
			//TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22); (counter, 1024)

			//Set top value
			//OCR2A = 210; //38khz (ir)

			//TCNT2 = 2;
			//OCR2A = 1; //About 30hz (counter)


			//Enable timer2 comp interrupt
			//on the specific timer you want 
			//to use :
			//TIMSK2 = (1 << OCIE2A); (ir)
			//TIMSK2 = (1 << TOIE2); (counter)

		break;
		
	}
}

void Pwm::stopPwm(){
	switch(counter_){
	case CNT0:
		TCCR0A &= ~(1 << COM0A0);
		break;
	case CNT1:
		TCCR1A = 0x00; //TODO set appropriate bit to 0
		break;
	case CNT2:
		TIMSK2 &= ~(1 << OCIE2A);
		break;
	}
}

void Pwm::startPwm(){
	switch(counter_){
	case CNT0:
		TCCR0A |= (1 << COM0A0);
		break;
	case CNT1:
		TCCR1A = 0x00; //TODO set appropriate bit to 1
		break;
	case CNT2:
		TIMSK2 |= (1 << OCIE2A);
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


