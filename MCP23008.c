// implementation for MCP23008
// Simon Walker, NAIT

#include "I2C.h"
#include <avr/io.h>
#include "MCP23008.h"

// private helpers
int MCP23008_ReadReg8 (unsigned char TargetRegister, unsigned char * TargetValue)
{
  if (I2C_Start(MCP23008ADDR, I2C_WRITE))
    return -1;
  
  // write register address
  if (I2C_Write8(TargetRegister, I2C_NOSTOP))
    return -2;
  
  if (I2C_Start(MCP23008ADDR, I2C_READ))
    return -3;
  
  if (I2C_Read8(TargetValue, I2C_NACK, I2C_STOP))
    return -4;
  
  return 0;
}

int MCP23008_WriteReg8 (unsigned char TargetRegister, unsigned char TargetValue)
{
  if (I2C_Start(MCP23008ADDR, I2C_WRITE))
  return -1;
  
  // write register address
  if (I2C_Write8(TargetRegister, I2C_NOSTOP))
  return -2;
  
  if (I2C_Write8(TargetValue, I2C_STOP))
  return -4;
  
  return 0;
}


// end private helpers

// public functions
int MCP23008_Init (void)
{
	// made it here, no error
	return 0;  
}

int MCP23008_ReadPort (unsigned char * PortValue)
{    
  return MCP23008_ReadReg8(MCP23008_REG_GPIO, PortValue);
}

int MCP23008_ReadINTCap (unsigned char * PortValue)
{
  // this should clear interrupt
  return MCP23008_ReadReg8(MCP23008_REG_INTCAP, PortValue);
}

// set weak pull-ups for pins
int MCP23008_SetWeakPullups (unsigned char PinMask)
{
  if (MCP23008_WriteReg8(MCP23008_REG_GPPU, PinMask))
    return -1;

  return 0;
}

int MCP23008_SetInterrupt (unsigned char PinMask)
{
  // odd, turn this on and the device freezes on the first interrupt
  //  needs additional investigation...
  // want to compare to default value for single (rising edge) trigger
  //if (MCP23008_WriteReg8(MCP23008_REG_INTCON, PinMask))
  //  return -1;

  // enable interrupts for the requested input pins
  if (MCP23008_WriteReg8(MCP23008_REG_GPINTEN, PinMask))
    return -2;

  // defval register OK (default value 0), so interrupt on rising edge

  // made it here, no error
  return 0;
}



