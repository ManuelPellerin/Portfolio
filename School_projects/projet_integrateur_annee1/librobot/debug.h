
#include "uart.h"

#ifdef DEBUG

# define DEBUG_PRINT(x) uartPrint(x) // ou par RS-232

#else

# define DEBUG_PRINT(x) do {} while (0) // code mort

#endif


