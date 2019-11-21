
#include "getNextInstruction.h"


void getNextInstruction(uint8_t& instruction, uint8_t& operand, uint16_t& index){
	
	Memoire24CXXX memory;
	memory.lecture(index, &instruction);
	index++;
	memory.lecture(index, &operand);
	index++;
}