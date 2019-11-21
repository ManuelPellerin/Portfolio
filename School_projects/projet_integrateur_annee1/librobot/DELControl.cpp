#include "DELControl.h"


void DELControl(uint8_t port, uint8_t pin, bool on) {

	pinMode(port, pin, OUTPUT);

    if (on)
    {
		digitalWrite(port, pin, ON);
    }
    else
    {
		digitalWrite(port, pin, OFF);
    }
    
}
