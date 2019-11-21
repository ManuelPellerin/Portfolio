
#include "debug.h"
#include "Transmitter.h"
#include <util/delay.h>

int main() {

	uartInit();

	// Output led on PC6
	DDRC |= (1 << PC6);

	Transmitter t;

	while(true){
		uint8_t theAmazingCode = (t.getCode() % 9);
		PORTC |= (1 << PC6);
		//45 ms loop, 45 loop for 2 secs
		for(uint8_t i = 0; i < 45; i++){
			t.sendPacket(theAmazingCode, 1);
		}
		PORTC &= ~(1 << PC6);
	}
	
}
