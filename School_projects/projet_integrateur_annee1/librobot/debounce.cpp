#include "debounce.h"


static int debounceDelayMs = 10; // amount of time in miliseconds between the initial check and the second check
bool debounceCheck(uint8_t port, uint8_t pin, bool buttonReversed)
{
	if (buttonReversed) {
		while (digitalRead(port, pin) == 0) {
			_delay_ms(debounceDelayMs);
			if (digitalRead(port, pin) == 0)
				return true;
			else
				return false;
		}
		return false;
	}
	else {
		while (digitalRead(port, pin)) {
			_delay_ms(debounceDelayMs);
			if (digitalRead(port, pin))
				return true;
			else
				return false;
		}
	}
	return false;
}
