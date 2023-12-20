// implementation for ADXL343
// Simon Walker, NAIT
// this accelerometer is weird in my mind
//  gravity, pointing down through the axis is read as positive
//  not sure if all devices work this way, but today, at least, this seems odd
#include "I2C.h"
#include <avr/io.h>
#include "ADXL343.h"

// private helpers
int ADXL343_ReadReg8 (unsigned char TargetRegister, unsigned char * TargetValue)
{
  if (I2C_Start(ADXL343ADDR, I2C_WRITE))
    return -1;
  
  // write register address
  if (I2C_Write8(TargetRegister, I2C_NOSTOP))
    return -2;
  
  if (I2C_Start(ADXL343ADDR, I2C_READ))
    return -3;
  
  if (I2C_Read8(TargetValue, I2C_NACK, I2C_STOP))
    return -4;
  
  return 0;
}

int ADXL343_WriteReg8 (unsigned char TargetRegister, unsigned char TargetValue)
{
  if (I2C_Start(ADXL343ADDR, I2C_WRITE))
  return -1;
  
  // write register address
  if (I2C_Write8(TargetRegister, I2C_NOSTOP))
  return -2;
  
  if (I2C_Write8(TargetValue, I2C_STOP))
  return -4;
  
  return 0;
}


// end private helpers

int ADXL343_ReadRegAccData (int * iX, int * iY, int * iZ)
{
  if (I2C_Start(ADXL343ADDR, I2C_WRITE))
    return -1;
  
  // write register address
  if (I2C_Write8(0x32, I2C_NOSTOP))
    return -2;
  
  if (I2C_Start(ADXL343ADDR, I2C_READ))
    return -3;
  
  unsigned int iTemp;
  
  if (I2C_Read8((unsigned char *)&iTemp, I2C_ACK, I2C_NOSTOP))
    return -4;      
  if (I2C_Read8(((unsigned char *)&iTemp) + 1, I2C_ACK, I2C_NOSTOP))
    return -4;
  
  *iX = (int)iTemp;
  
  if (I2C_Read8((unsigned char *)&iTemp, I2C_ACK, I2C_NOSTOP))
	return -4;
  if (I2C_Read8(((unsigned char *)&iTemp) + 1, I2C_ACK, I2C_NOSTOP))
	return -4;
  
  *iY = (int)iTemp;
  
  if (I2C_Read8((unsigned char *)&iTemp, I2C_ACK, I2C_NOSTOP))
	return -4;
  if (I2C_Read8(((unsigned char *)&iTemp) + 1, I2C_NACK, I2C_STOP))
	return -4;
  
  *iZ = (int)iTemp;
    
   return 0;
}

// public functions
int ADXL343_Init (void)
{
  unsigned char devid = 0;
  
  // in the case of restart, put the device to sleep before changes
  if (ADXL343_WriteReg8(0x2D, 0b00000100))
    return -1;
    
  // in the case of restart, turn off all interrupts
  if (ADXL343_WriteReg8(0x2E, 0b00000000)) // enable activity interrupt 		
    return -2;
  
  if (ADXL343_ReadReg8 (0, &devid))
    return -3;
  
  if (devid != 0b11100101)  // fixed dev id
    return -4;
    
  // 100Hz, normal operation is the default (register 0x3c)
  ADXL343_WriteReg8(0x31, 0b00000000);
  //ADXL343_WriteReg8(0x31, 0b00000100); // change to MSB justification
  ADXL343_WriteReg8(0x38, 0b00000000);
  
  // go for measurements
  ADXL343_WriteReg8 (0x2D, 0b00001000);
    
	// made it here, no error
	return 0;  
}

// testing, enable activity interrupt on interrupt 1 (parameterize later)
void ADXL343_ActivityInterrupt (void)
{
	// assume device is initialized
	ADXL343_WriteReg8(0x24, 2); // 2 * 62.5mg
	ADXL343_WriteReg8(0x27, 0b11110000); // act ac, xyz, no inact
	ADXL343_WriteReg8(0x2F, 0b00000000); // all interrupts to int1
	ADXL343_WriteReg8(0x2E, 0b00010000); // enable activity interrupt 		
}

unsigned char ADXL343_ReadInterruptSource (void)
{
	unsigned char target = 0;
	ADXL343_ReadReg8(0x30, &target);
	return target;
}

