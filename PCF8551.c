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

void PCF8551_DisplayOn (void)
{
	PCF8551_Write(0x02, 0b00000001); // display on		
}

void PCF8551_DisplayOff (void)
{
	PCF8551_Write(0x02, 0b00000000); // display off
}

void PCF8551_Test (void)
{
	PCF8551_Write(0x04, 0b00000000); // random segs
	PCF8551_Write(0x05, 0b00000000); // random segs
	PCF8551_Write(0x06, 0b00000000); // random segs
	PCF8551_Write(0x07, 0b00000000); // random segs
	PCF8551_Write(0x08, 0b00000000); // random segs
	PCF8551_Write(0x09, 0b00000000); // random segs
	PCF8551_Write(0x0a, 0b00000000); // random segs
	PCF8551_Write(0x0b, 0b00000000); // random segs
	PCF8551_Write(0x0c, 0b00000000); // random segs
	PCF8551_Write(0x0d, 0b00000000); // random segs
	PCF8551_Write(0x0e, 0b00000000); // random segs
	PCF8551_Write(0x0f, 0b00000000); // random segs
	PCF8551_Write(0x10, 0b00000000); // random segs
	PCF8551_Write(0x11, 0b00000000); // random segs
	PCF8551_Write(0x12, 0b00000000); // random segs
	PCF8551_Write(0x13, 0b00000000); // random segs
	PCF8551_Write(0x14, 0b00000000); // random segs
	PCF8551_Write(0x15, 0b00000000); // random segs
	PCF8551_Write(0x16, 0b00000000); // random segs	
}


