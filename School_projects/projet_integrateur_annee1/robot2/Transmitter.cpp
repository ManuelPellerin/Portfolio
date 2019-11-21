#include "Transmitter.h"
#include "debug.h"

volatile uint8_t buttonState = 0x00;
volatile uint8_t codeCounter = 0x00;
volatile uint16_t timeout = 1000;

ISR(INT0_vect){
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
//test
void Transmitter::test(){
	pwm_.startPwm();
	_delay_ms(1.2);
	pwm_.stopPwm();
	_delay_ms(0.6);
}
//endtest

Transmitter::Transmitter()
	:pwm_(53, 0, CNT1) {
	pwm_.stopPwm();
	cli();
	//set the interrupt on int0
	EIMSK |= 1 << INT0;
  	EICRA |= 1 << ISC00;
  	DDRD &= ~(1 << 2);
  	DDRA |= (1);
}

uint8_t Transmitter::getCode() {

	sei();
	//wait for the first press before
	//starting the timeout
	while(codeCounter == 0);
	//start the timeout
	while(timeout > 0){
		timeout--;
		_delay_ms(1);
	}
	cli();
	uint8_t finalCode = codeCounter;
	codeCounter = 0;

	return finalCode;
}

void Transmitter::sendBit(uint8_t& bit, uint8_t& waitCounter) {

	if (bit == 0x01) {
		pwm_.startPwm();
		_delay_ms(1.2);
		pwm_.stopPwm();
		_delay_ms(0.6);
		waitCounter += 3;
	}
	else {
		pwm_.startPwm();
		_delay_ms(0.6);
		pwm_.stopPwm();
		_delay_ms(0.6);
		waitCounter += 2;
	}
}


void Transmitter::sendPacket(uint8_t packet, uint8_t address) {

	uint8_t waitCounter = 5;

	//send the start bit
	pwm_.startPwm();
	_delay_ms(2.4);
	pwm_.stopPwm();
	_delay_ms(0.6);

	//send the command
	for(uint8_t i = 0; i < 7; i++){
		//seperate the wanted bit, lsb first
		uint8_t packetBit = (packet >> i) & 0x01;

		sendBit(packetBit, waitCounter);
	}

	//send the address
	for(uint8_t i = 0; i < 5; i++){
		//seperate the wanted bit, lsb first
		uint8_t addressBit = (address >> i) & 0x01;

		sendBit(addressBit, waitCounter);
	}

	//wait some extra time for a 45ms period
	for(uint8_t i = 0; i < (75 - waitCounter); i++){
		_delay_ms(0.6);
	}


}