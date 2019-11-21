#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "Pwm.h"
#include <util/delay.h>

class Transmitter {
public:
	Transmitter();

	uint8_t getCode();
	void sendBit(uint8_t& bit, uint8_t& waitCounter);
	void sendPacket(uint8_t packet, uint8_t address);
	//test
	void test();
	//endtest

private:
	Pwm pwm_;
};

#endif