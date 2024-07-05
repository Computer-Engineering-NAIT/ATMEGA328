// header for PCF85551 - LCD Driver
// Simon Walker, NAIT
// Revision History:
// July 4 2024 - Initial Build

//int PCF8551_Write (unsigned char ucAddr, unsigned char ucData);
//int PCF8551_Read (unsigned char * Target);

int PCF8551_Init (void);
void PCF8551_DisplayOn (void);
void PCF8551_DisplayOff (void);
void PCF8551_Test (void);
