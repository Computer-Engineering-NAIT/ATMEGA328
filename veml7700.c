// implementation for VEML7700
// Simon Walker, NAIT

#include "I2C.h"
#include <avr/io.h>
#include "veml7700.h"

int VEML7700_ReadWhite (unsigned int * uiWhite)
{
	unsigned char ucDataH, ucDataL;

	// start transaction with device address
	if (I2C_Start(0x10, I2C_WRITE))
		return -1;
	
	// want to read WHITE level, addr 5
	if (I2C_Write8(0x05, I2C_NOSTOP))
		return -2;
	
	if (I2C_Start(0x10, I2C_READ))
		return -3;

	if (I2C_Read8(&ucDataL, I2C_ACK, I2C_NOSTOP))
		return -4;

	if (I2C_Read8(&ucDataH, I2C_NACK, I2C_STOP))
		return -5;

	*uiWhite = ucDataH * 256 + ucDataL;
	
	return 0;
}

int VEML7700_ReadAmbient (unsigned int * uiAmbient)
{
	unsigned char ucDataH, ucDataL;

	// start transaction with device address
	if (I2C_Start(0x10, I2C_WRITE))
	return -1;
	
	// want to read WHITE level, addr 4
	if (I2C_Write8(0x04, I2C_NOSTOP))
	return -2;
	
	if (I2C_Start(0x10, I2C_READ))
	return -3;

	if (I2C_Read8(&ucDataL, I2C_ACK, I2C_NOSTOP))
	return -4;

	if (I2C_Read8(&ucDataH, I2C_NACK, I2C_STOP))
	return -5;

	*uiAmbient = ucDataH * 256 + ucDataL;
	
	return 0;
}

// by default, the device is in shutdown, so make it alive
//  and bring it up with conservative sampling rates
int VEML7700_Init (void)
{
	// gain of 1
	// integration time 400ms
	// persistence protect 1
	// interrupt off
	// power on
	unsigned int uiSettings = 0b0000000010000000;

	// start transaction with device address
	if (I2C_Start(0x10, I2C_WRITE))
		return -1;

	// want to write device config (register 0, 16-bits)
	if (I2C_Write8(0x00, I2C_NOSTOP))
		return -2;

	if (I2C_Write8((unsigned char)uiSettings, I2C_NOSTOP))
		return -3;

	if (I2C_Write8((unsigned char)(uiSettings >> 8), I2C_STOP))
		return -4;	

	// made it here, no error
	return 0;
}