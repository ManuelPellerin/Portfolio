#include "binToEeprom.h"
#include "debug.h"

void binToEeprom() {
	bool dataReceived = true;

	Memoire24CXXX memory;
	uint16_t address = 0x00;
	while (dataReceived == true) {

		// Get the data from the uart receive register
		uint8_t data = 0x00;
		uartReception(data, dataReceived);

		// Put the data in memory
		memory.ecriture(address, data);
		address++;
		
		char msg[41] = "mem_debug\n";
		DEBUG_PRINT((msg));
	}
}