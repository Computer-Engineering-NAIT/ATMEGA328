// header for ADXL343, 7-bit address 0x53
// Simon Walker, NAIT
// Revision History:

#ifndef ADXL343ADDR
#define ADXL343ADDR 0x53
#endif

int ADXL343_Init (void);
int ADXL343_ReadRegAccData (int * iX, int * iY, int * iZ);
void ADXL343_ActivityInterrupt (void);

unsigned char ADXL343_ReadInterruptSource (void);

// private helpers
// int ADXL343_ReadReg8 (unsigned char TargetRegister, unsigned char * TargetValue);
// int ADXL343_WriteReg8 (unsigned char TargetRegister, unsigned char TargetValue);