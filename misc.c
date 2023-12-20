// implementation for misc functions (general re-use)
// Simon Walker, NAIT

#include <avr/io.h>
#include "misc.h"
#include <stdio.h>

// support function to turn ms into wall time
void TimeFromMS (char * buff, unsigned long ticks)
{
  (void)sprintf (buff, "%2.2d:%2.2d:%2.2d:%2.2d.%1.1d", // DD:HH:MM:SS:T
  (unsigned int)(ticks / (1000l * 60 * 60 * 24)),       // DD
  (char)((ticks / (1000l * 60 * 60)) % 24),             // HH
  (char)((ticks / (1000l * 60)) % 60),                  // MM
  (char)((ticks / 1000l) % 60),                         // SS
  (char)(ticks / 100 % 10)                              // H
  );
}

void ToBin8 (char * target, unsigned char value)
{
  int i = 0;
  for (unsigned char mask = 0x80; mask; mask >>= 1, ++i)
  {
    target[i] = mask & value ? '1' : '0';
  }
  target[i] = 0;
}


