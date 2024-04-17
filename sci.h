// SCI library, ATmega328P Version
// Simon Walker, NAIT
// Revision History:
// March 18 2022 - Initial Build

// what the ISR should look like (copy to implementation)
/*
ISR (USART_RX_vect)
{
  // indicate byte has been received (and can detect overrun too)
  ++_bRXFlag;
  
  // pull the byte (clears the interrupt too)
  _RXByte = UDR0;
}
*/

// initialize UCSR0 for asynchronous use, 8N1, at specified BAUD rate
// NOTE: Running 9600 BAUD with a clock of 1MHz is not recommended
//  depending on technique - 8928 or 10416 BAUD will be generated due
//  to low denom values - recommended that minimum clock is 2MHz
int SCI0_Init (unsigned long ulBus, unsigned long ulBAUD, int bRXInt);

// blocking send of a byte
void SCI0_BSend (unsigned char data);

// blocking send of a string
void SCI0_TxString (char * buff);

// blocking send of a 16-bit HEX value as 0x0000
void SCI0_Tx16H (unsigned int uiVal, int tl);

// zero on byte rxed, otherwise no byte to read
int SCI0_RxByte (unsigned char * pData);
