#include "Receiver.h"

#include <avr/interrupt.h>
#include "debug.h"

volatile bool beginReception = false;
volatile uint8_t irTimerCounter = 0;
volatile uint8_t finalCounter = 0;

ISR(INT1_vect){
	if (!((1 << PD3) & PIND)){
		finalCounter = 0;
		beginReception = true;
	}
	else{
		finalCounter = irTimerCounter;
		beginReception = false;
	}
}

ISR(TIMER2_COMPA_vect){
	if(beginReception){
		irTimerCounter += 1;
	}	
}

volatile bool LETS_STOP_RIGHT_HERE = false;
volatile uint8_t buttonState = 0x00;
volatile uint8_t codeCounter = 0x00;
volatile uint16_t timeout = 1000;

ISR(INT0_vect) {
	DEBUG_PRINT((22));
	LETS_STOP_RIGHT_HERE = true;

	//debouncer
	const int nCycles = 100; //0.1ms cycle
	int counter = 0;

	while(buttonState != (PIND & (1 << PD2))){
		if(counter < nCycles) {
			_delay_ms(0.1);
			counter++;
		}
		else {
			if(buttonState == 0x00) {
				buttonState = (1 << PD2);
				codeCounter++;
				timeout = 1000;
				
			}
			else {
				buttonState = 0x00;
			}
		}
	}
}

Receiver::Receiver()
	:counter_(0, 0, CNT2){
	cli();
	// Set the timer prescaler
	// Enable its interupt
	// Set top value
	TCCR2B = (1 << CS20);
	OCR2A = 210;
	TIMSK2 = (1 << OCIE2A);
	//set the interrupt on int0
	EIMSK |= 1 << INT0;
	EICRA |= 1 << ISC00;
  	DDRD &= ~(1 << 2);
  	DDRA |= (1);
	//set the interrupt on int1
	EIMSK |= 1 << INT1;
  	EICRA |= 1 << ISC10;
  	DDRD &= ~(1 << 3);
  	DDRA |= (1);
  	counter_.stopPwm();
  	sei();
}

uint16_t Receiver::getCodeFromInt0(){
	//wait for the first press before
	//starting the timeout
	while(codeCounter == 0);
	//start the timeout
	while(timeout > 0){
		timeout--;
		_delay_ms(1);
	}

	return (codeCounter | 0b10000000);
}

uint16_t Receiver::receivePacket(bool& buttonPressed){
	LETS_STOP_RIGHT_HERE = false;
	codeCounter = 0;
	uint16_t packet = 0;
	counter_.startPwm();

	//decode the pulse
	uint8_t i = 0;
	while(i < 13 && !LETS_STOP_RIGHT_HERE){
		//wait for stop bit
		while((beginReception || !irTimerCounter) && !LETS_STOP_RIGHT_HERE);
		//DEBUG_PRINT((finalCounter));
		irTimerCounter = 0;
		//check if we got the start bit
		if(i == 0) {
			//DEBUG_PRINT((finalCounter));
			if (finalCounter >= START_BIT_PULSE_COUNT){
				i++;
				DEBUG_PRINT((69));
			}
		}
		else {
			if (finalCounter >= ONE_BIT_PULSE_COUNT) {
				packet |= (1 << (i-1));
			}
			i++;
		}
	}
	counter_.stopPwm();

	if(LETS_STOP_RIGHT_HERE) {
		packet = getCodeFromInt0();
		buttonPressed = true;
		DEBUG_PRINT((34));
	}

	beginReception = false;
	LETS_STOP_RIGHT_HERE = false;

	return packet;
}

uint16_t Receiver::receiveInARow(uint8_t nPackets){
	
	uint16_t lastPacket = 0x00;
	uint8_t goodPacketCounter = 0;
	bool buttonPressed = false;
	while(goodPacketCounter < nPackets && !buttonPressed) {
		uint16_t packet = receivePacket(buttonPressed);
		if(packet == lastPacket) {
			goodPacketCounter++;
			DEBUG_PRINT((goodPacketCounter));
		}
		else {
			goodPacketCounter = 0;
			lastPacket = packet;
		}
	}
	return lastPacket;
}























