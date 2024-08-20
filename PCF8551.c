// implementation for PCF85551 - LCD Driver
// Simon Walker, NAIT

#include <avr/io.h>
#include <avr/pgmspace.h> // definitions to place items in flash
#include "I2C.h"
#include "PCF8551.h"

// debug
#include <stdio.h>
#include "sci.h"

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

// memory buffer for display data (written at once)
unsigned char PCF8551_DispRAM [20] = {0}; // offset to address 4 in device


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

int PCF8551_WriteRAM (void)
{
	if (I2C_Start(PCF8551_ADDR, I2C_WRITE))
	return -1;

	if (I2C_Write8(0x04, I2C_NOSTOP))
	return -2;
	
	for (int i = 0; i < 19; ++i)
	{
		if (I2C_Write8(PCF8551_DispRAM[i], I2C_NOSTOP))
		return -3;
	}

	if (I2C_Write8(PCF8551_DispRAM[19], I2C_STOP))
	return -4;
	
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

// public functions
int PCF8551_Init (void)
{	
	PCF8551_Write(0x01, 0b00000100); // device control, osc on, 64Hz COE off
	PCF8551_Write(0x02, 0b00000000); // defaults for display control (off)
	PCF8551_Write(0x03, 0b00000000); // display control, no blink		
	
	for (int i = 0; i < 20; ++i)	
	PCF8551_DispRAM[i] = 0;
	
	PCF8551_WriteRAM();
	
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

typedef struct PCF8551_SegMap
{
	unsigned char reg;
	unsigned char bit;	
} PCF8551_SegMap;

const PCF8551_SegMap _SegLookup [16 * 8] =
{
	// pos 0
	{ 0x15, 2 }, // A
	{ 0x10, 2 }, // B
	{ 0x0b, 2 }, // C
	{ 0x04, 1 }, // D
	{ 0x09, 1 }, // E
	{ 0x0E, 1 }, // F
	{ 0x10, 1 }, // G
	{ 0x15, 1 }, // H		
	{ 0x13, 2 }, // I				
	{ 0x0E, 2 }, // J						
	{ 0x09, 2 }, // K
	{ 0x04, 2 }, // L
	{ 0x06, 1 }, // M
	{ 0x0b, 1 }, // N
	{ 0x06, 2 }, // DP
	{ 0x13, 1 }, // CA
	// pos 1
	{ 0x15, 8 }, // A
	{ 0x10, 8 }, // B
	{ 0x0b, 8 }, // C
	{ 0x04, 4 }, // D
	{ 0x09, 4 }, // E
	{ 0x0E, 4 }, // F
	{ 0x10, 4 }, // G
	{ 0x15, 4 }, // H
	{ 0x13, 8 }, // I
	{ 0x0E, 8 }, // J
	{ 0x09, 8 }, // K
	{ 0x04, 8 }, // L
	{ 0x06, 4 }, // M
	{ 0x0b, 4 }, // N
	{ 0x06, 8 }, // DP
	{ 0x13, 4 }, // CA
	// pos 2
	{ 0x15, 32 }, // A
	{ 0x10, 32 }, // B
	{ 0x0b, 32 }, // C
	{ 0x04, 16 }, // D
	{ 0x09, 16 }, // E
	{ 0x0E, 16 }, // F
	{ 0x10, 16 }, // G
	{ 0x15, 16 }, // H
	{ 0x13, 32 }, // I
	{ 0x0E, 32 }, // J
	{ 0x09, 32 }, // K
	{ 0x04, 32 }, // L
	{ 0x06, 16 }, // M
	{ 0x0b, 16 }, // N
	{ 0x06, 32 }, // DP
	{ 0x13, 16 }, // CA
	// pos 3
	{ 0x15, 128 }, // A
	{ 0x10, 128 }, // B
	{ 0x0b, 128 }, // C
	{ 0x04, 64 }, // D
	{ 0x09, 64 }, // E
	{ 0x0E, 64 }, // F
	{ 0x10, 64 }, // G
	{ 0x15, 64 }, // H
	{ 0x13, 128 }, // I
	{ 0x0E, 128 }, // J
	{ 0x09, 128 }, // K
	{ 0x04, 128 }, // L
	{ 0x06, 64 }, // M
	{ 0x0b, 64 }, // N
	{ 0x06, 128 }, // DP
	{ 0x13, 64 }, // CA
	// pos 4
	{ 0x16, 2 }, // A
	{ 0x11, 2 }, // B
	{ 0x0C, 2 }, // C
	{ 0x05, 1 }, // D
	{ 0x0A, 1 }, // E
	{ 0x0F, 1 }, // F
	{ 0x11, 1 }, // G
	{ 0x16, 1 }, // H
	{ 0x14, 2 }, // I
	{ 0x0F, 2 }, // J
	{ 0x0A, 2 }, // K
	{ 0x05, 2 }, // L
	{ 0x07, 1 }, // M
	{ 0x0C, 1 }, // N
	{ 0x07, 2 }, // DP
	{ 0x14, 1 }, // CA
	// pos 5
	{ 0x16, 8 }, // A
	{ 0x11, 8 }, // B
	{ 0x0C, 8 }, // C
	{ 0x05, 4 }, // D
	{ 0x0A, 4 }, // E
	{ 0x0F, 4 }, // F
	{ 0x11, 4 }, // G
	{ 0x16, 4 }, // H
	{ 0x14, 8 }, // I
	{ 0x0F, 8 }, // J
	{ 0x0A, 8 }, // K
	{ 0x05, 8 }, // L
	{ 0x07, 4 }, // M
	{ 0x0C, 4 }, // N
	{ 0x07, 8 }, // DP
	{ 0x14, 4 }, // CA
	// pos 6
	{ 0x16, 32 }, // A
	{ 0x11, 32 }, // B
	{ 0x0C, 32 }, // C
	{ 0x05, 16 }, // D
	{ 0x0A, 16 }, // E
	{ 0x0F, 16 }, // F
	{ 0x11, 16 }, // G
	{ 0x16, 16 }, // H
	{ 0x14, 32 }, // I
	{ 0x0F, 32 }, // J
	{ 0x0A, 32 }, // K
	{ 0x05, 32 }, // L
	{ 0x07, 16 }, // M
	{ 0x0C, 16 }, // N
	{ 0x07, 32 }, // DP
	{ 0x14, 16 }, // CA
	// pos 7
	{ 0x16, 128 }, // A
	{ 0x11, 128 }, // B
	{ 0x0C, 128 }, // C
	{ 0x05, 64 }, // D
	{ 0x0A, 64 }, // E
	{ 0x0F, 64 }, // F
	{ 0x11, 64 }, // G
	{ 0x16, 64 }, // H
	{ 0x14, 128 }, // I
	{ 0x0F, 128 }, // J
	{ 0x0A, 128 }, // K
	{ 0x05, 128 }, // L
	{ 0x07, 64 }, // M
	{ 0x0C, 64 }, // N
	{ 0x07, 128 }, // DP
	{ 0x14, 64 } // CA				
};

void PCF8551_SegTog (int iPos, PCF8551_Seg seg)
{
	// map individual segment at desired position to memory array
	// since the mapping is quite arbitrary, this will help with higher-level functions
	// positions are 0-7 as left to right address on display, segs from datasheet
	// this toggles the segment, and does not alter any other bits
	if (iPos < 0 || iPos > 7)
		return;
		
	// find pointer into the lookup table
	int iOffset = iPos * 16 + seg;
	
	// grab the bit that applies for this segment/position
	PCF8551_SegMap data = _SegLookup[iOffset];
	
	// toggle that bit in the display image, normalizing array/register position
	PCF8551_DispRAM[data.reg - 4] ^= data.bit;	
}

void PCF8551_Test (void)
{
	//PCF8551_SegOn (0, SEG_A);
	//PCF8551_SegOn (0, SEG_D);
	//PCF8551_SegOn (0, SEG_B);
	//PCF8551_SegOn (0, SEG_C);
	//PCF8551_SegOn (0, SEG_I);
	//PCF8551_SegOn (0, SEG_K);
	//PCF8551_SegOn (0, SEG_M);
//
	//PCF8551_SegOn (1, SEG_A);
	//PCF8551_SegOn (1, SEG_D);
	//PCF8551_SegOn (1, SEG_F);
	//PCF8551_SegOn (1, SEG_E);
	//PCF8551_SegOn (1, SEG_B);
	//PCF8551_SegOn (1, SEG_C);	
//
	//PCF8551_SegOn (2, SEG_A);
	//PCF8551_SegOn (2, SEG_D);
	//PCF8551_SegOn (2, SEG_F);
	//PCF8551_SegOn (2, SEG_E);
	//PCF8551_SegOn (2, SEG_B);
	//PCF8551_SegOn (2, SEG_C);
//
	//PCF8551_SegOn (3, SEG_A);
	//PCF8551_SegOn (3, SEG_D);
	//PCF8551_SegOn (3, SEG_B);
	//PCF8551_SegOn (3, SEG_C);
	//PCF8551_SegOn (3, SEG_I);
	//PCF8551_SegOn (3, SEG_K);
	//PCF8551_SegOn (3, SEG_M);
//
	//PCF8551_SegOn (4, SEG_A);
	//PCF8551_SegOn (4, SEG_D);
	//PCF8551_SegOn (4, SEG_J);
	//PCF8551_SegOn (4, SEG_N);
	
	PCF8551_SegTog (2, SEG_A);
	PCF8551_SegTog (2, SEG_F);
	PCF8551_SegTog (2, SEG_E);
	PCF8551_SegTog (2, SEG_D);

	PCF8551_SegTog (3, SEG_E);
	PCF8551_SegTog (3, SEG_F);
	PCF8551_SegTog (3, SEG_H);
	PCF8551_SegTog (3, SEG_L);
	PCF8551_SegTog (3, SEG_B);
	PCF8551_SegTog (3, SEG_C);
	
	PCF8551_SegTog (4, SEG_A);
	PCF8551_SegTog (4, SEG_I);
	PCF8551_SegTog (4, SEG_M);
	
	PCF8551_WriteRAM();	
}


