#ifndef GET_NEXT_INSTRUCTION_H
#define GET_NEXT_INSTRUCTION_H

#include <avr/io.h>
#include "memoire_24.h"

void getNextInstruction(uint8_t& instruction, uint8_t& operand, uint16_t& index);


#endif