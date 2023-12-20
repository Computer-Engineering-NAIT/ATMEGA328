// header for MCP23008, 7-bit address 0x20
// Simon Walker, NAIT
// Revision History:

#ifndef MCP23008ADDR
#define MCP23008ADDR 0x20
#endif

#define MCP23008_REG_IODIR 0
#define MCP23008_REG_IPOL 1
#define MCP23008_REG_GPINTEN 2
#define MCP23008_REG_DEFVAL 3
#define MCP23008_REG_INTCON 4
#define MCP23008_REG_IOCON 5
#define MCP23008_REG_GPPU 6
#define MCP23008_REG_INTF 7
#define MCP23008_REG_INTCAP 8
#define MCP23008_REG_GPIO 9
#define MCP23008_REG_OLAT 10

int MCP23008_Init (void);

// read the pin states
int MCP23008_ReadPort (unsigned char * PortValue);

// read the captured state of the port at time of interrupt
int MCP23008_ReadINTCap (unsigned char * PortValue);

// set interrupt mask for input pins
int MCP23008_SetInterrupt (unsigned char PinMask);

// set weak pullups for pins
int MCP23008_SetWeakPullups (unsigned char PinMask);

// private helpers
int MCP23008_ReadReg8 (unsigned char TargetRegister, unsigned char * TargetValue);
int MCP23008_WriteReg8 (unsigned char TargetRegister, unsigned char TargetValue);