#include <debug.h>
#include <util/delay.h>
#include <LineFollower.h>
#include <Receiver.h>

int main() {

	uartInit();
	
	LineFollower lineFollower;
	lineFollower.calibrate();
	
	// Get the initial code
	uint8_t initialCode = 0;
	Receiver irReceiver;
	while ((initialCode < 1) || (initialCode > 4)) {
		uint16_t packet = irReceiver.receiveInARow();
		// Isolate the first 7 bits of the packet
		initialCode = packet & 0x7f;
	}
	DEBUG_PRINT((initialCode));
	lineFollower.setCurrentSegment(initialCode);

	// Display the code for 3 seconds
	PORTC |= (1 << (initialCode + 1));
	_delay_ms(3000);
	PORTC &= ~(1 << (initialCode + 1));

	// Do the 4 segments
	for (uint8_t i = 0; i < 4; i++) {
		lineFollower.nextMeme();
	}
	while (true) {
	}
}