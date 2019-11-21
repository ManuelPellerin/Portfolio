#ifndef RECEIVER_T
#define RECEIVER_T
#include <avr/interrupt.h>
#include <avr/io.h> 
#include "debug.h"
#include "Pwm.h"
#define START_BIT_PULSE_COUNT 90 //actual value : 96, exp
#define ONE_BIT_PULSE_COUNT 45 //actual value : 48 exp 


//extern volatile uint8_t irPulseCounter;
//extern volatile uint8_t irTimerCounter;

class Receiver {
public:
	Receiver();
	uint16_t receivePacket(bool& buttonPressed);
	uint16_t receiveInARow(uint8_t nPackets = 3);
	uint16_t getCodeFromInt0();

private:
	Pwm counter_;
};

#endif