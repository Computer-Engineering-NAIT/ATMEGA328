// I2C library, ATmega328PB Version
// Simon Walker, NAIT

// the PB version appears to need to differentiate between TWI modules
//  as there are, in fact, two on the PB device

#include <avr/io.h>
#include "I2C.h"

// not sure why there is a prescale greater than 1, as the bus rate
//  won't typically be greater than 16MHz, and the I2C rate won't
//  be slower than 100kHz, unless the user wants to run the I2C rate
//  much slower than 100kHz?
// return -1 if rate unreachable
int I2C_Init (unsigned long ulBusRate, I2C_BusRate sclRate)
{
	// start will power off all modules...
	// ensure power is on : TWI
	PRR0 &= 0b01111111;

	float fac = 0;

	switch (sclRate)
	{
		case I2CBus100:
			fac = ((ulBusRate / 2.0f) / 100000.0f) - 8;
			break;
		case I2CBus400:
			fac = ((ulBusRate / 2.0f) / 400000.0f) - 8;
			break;
	}

	// fac must fit into 8 bits
	if (fac < 1 || fac > 255)
		return -1;

	// set rate
	TWBR0 = (unsigned char)fac;

	// power on I2C to grab module pins
	TWCR0 |= 0b00000100;

	return 0;
}

// assume 128-byte buffer provided for scan results
void I2C_Scan (unsigned char * results)
{
	for (unsigned char addr = 0x01; addr <= 0x7E; ++addr)
	{
		if (!I2C_Start(addr, I2C_WRITE))
			results[addr] = addr;			
		else
			results[addr] = 0;				

		// send stop
		TWCR0 = 0b10010100;

		// wait for stop to automatically clear (stop completed)
		while (TWCR0 & 0x10)
		;
	}
}

int I2C_Start (unsigned char uc7Addr, int bRead)
{
	// send start
	TWCR0 = 0b10100100;
	
	// wait for operation to complete
	while (!(TWCR0 & 0x80))
	;

	// ensure status says START sent (or restart?)
	if (!((TWSR0 & 0b11111000) == 0x08 || (TWSR0 & 0b11111000) == 0x10))
	  return -1;

	// now send address with read or write
	if (bRead)
	{
		// enter master read mode
		TWDR0 = (uc7Addr << 1) | 0x01;

		// clear TWINT, no START, keep TWI enabled
		TWCR0 = 0b10000100;
		
		// wait for operation to complete
		while (!(TWCR0 & 0x80))
		;

		// look for ADDR+R sent with ACK
		if ((TWSR0 & 0b11111000) != 0x40)
		return -2;
	}
	else
	{
		// enter master write mode
		TWDR0 = uc7Addr << 1;

		// clear TWINT, no START, keep TWI enabled
		TWCR0 = 0b10000100;
		
		// wait for operation to complete
		while (!(TWCR0 & 0x80))
		;

		// look for ADDR+W sent with ACK
		if ((TWSR0 & 0b11111000) != 0x18)
		return -2;
	}

	return 0;
}

// assumes transaction is open
int I2C_Read8 (unsigned char *ucData, int bAck, int bStop)
{
	// clear TWINT, keep TWI enabled
	if (bAck)
	TWCR0 = 0b11000100;
	else
	TWCR0 = 0b10000100;

	// look for data sent, with TWINT bit
	while (!(TWCR0 & 0x80))
	;

	if (bAck)
	{
		// look for data received, ack returned
		if ((TWSR0 & 0b11111000) != 0x50)
		return -3;
	}
	else
	{
		// look for data received, ack not returned
		if ((TWSR0 & 0b11111000) != 0x58)
		return -3;
	}

	// read the data byte
	*ucData = TWDR0;
	
	// if stop requested, send it
	if (bStop)
	{
		// send STOP
		TWCR0 = 0b10010100;

		// wait for stop to automatically clear (stop completed)
		while (TWCR0 & 0x10)
			;
	}

	return 0;
}

// assumes transaction is open
int I2C_Write8 (unsigned char ucData, int bStop)
{
	// enter master write mode
	TWDR0 = ucData;

	// clear TWINT, no START, keep TWI enabled
	TWCR0 = 0b10000100;
	
	// look for data sent, with TWINT bit
	while (!(TWCR0 & 0x80))
	;

	// look for data sent with ACK
	if ((TWSR0 & 0b11111000) != 0x28)
	return -3;
	
	// if stop requested, send it
	if (bStop)
	{
		// send STOP
		TWCR0 = 0b10010100;

		// wait for stop to automatically clear (stop completed)
		while (TWCR0 & 0x10)
			;
	}

	return 0;
}

