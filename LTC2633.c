// implementation for LTC2633 Dual DAC
// Simon Walker, NAIT

#include <avr/io.h>
#include "I2C.h"
#include "LTC2633.h"

int LTC2633_Write (unsigned int uiVal)
{
	if (I2C_Start(0x10, false))
	return -1;
	if (I2C_Write8 (0b00100000, 0)) // channel A
	return -2;
	if (I2C_Write8 ((unsigned char)(uiVal >> 4), 0)) // data
	return -3;
	if (I2C_Write8 ((unsigned char)(uiVal << 4), 1)) // data
	return -4;

	return 0;
}
