// header for PCF85551 - LCD Driver
// Simon Walker, NAIT
// Revision History:
// July 4 2024 - Initial Build

//int PCF8551_Write (unsigned char ucAddr, unsigned char ucData);
//int PCF8551_Read (unsigned char * Target);

typedef enum PCF8551_Seg
{
	SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_H, SEG_I, SEG_J, SEG_K, SEG_L, SEG_M, SEG_N, SEG_DP, SEG_CA  
} PCF8551_Seg;

int PCF8551_Init (void);
void PCF8551_DisplayOn (void);
void PCF8551_DisplayOff (void);
void PCF8551_SegTog (int iPos, PCF8551_Seg seg);
int PCF8551_WriteRAM (void);


void PCF8551_Test (void);
