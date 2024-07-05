// implementation for PCF85551 - LCD Driver
// Simon Walker, NAIT

#include <avr/io.h>
#include "I2C.h"
#include "PCF8551.h"

#define PCF8551_ADDR 0x38

/*
#define I2C_STOP 1
#define I2C_NOSTOP 0
#define I2C_READ 1
#define I2C_WRITE 0
#define I2C_ACK 1
#define I2C_NACK 0
*/

// private helpers
int PCF8551_Write (unsigned char ucAddr, unsigned char ucData)
{
	if (I2C_Start(PCF8551_ADDR, I2C_WRITE))
	return -1;

	if (I2C_Write8(ucAddr, I2C_NOSTOP))
	return -2;

	if (I2C_Write8(ucData, I2C_STOP))
	return -3;
	
	return 0;
}

int PCF8551_Read (unsigned char * Target)
{
	if (I2C_Start(PCF8551_ADDR, I2C_READ))
	return -1;

	if (I2C_Read8(Target, I2C_NACK, I2C_STOP))
	return -2;
	
	return 0;
}

int PCF8551_Init (void)
{	
	PCF8551_Write(0x01, 0b00000100); // device control, osc on, 64Hz COE off
	PCF8551_Write(0x02, 0b00000000); // defaults for display control (off)
	PCF8551_Write(0x03, 0b00000000); // display control, no blink
	
	return 0;		
}

