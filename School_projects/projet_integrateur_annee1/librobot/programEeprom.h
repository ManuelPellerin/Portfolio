
#include "binToEeprom.h"

#ifdef PROG_EEPROM

#define PROGRAM_EEPROM() binToEeprom()

#else

#define PROGRAM_EEPROM() do {} while (0) // code mort

#endif
