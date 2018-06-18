/*
 * cli_print.c
 *
 */

#include "cli_print.h"

/***************************************************
 * print
 *
 ***************************************************/
void print(char *string)
{
  ACCESS(USART2_DR) &= ~(0xFF);
  
  while(*string)
	{
		// See pg 548 of the ST RM0383 document for more info on the USART registers.
		while((ACCESS(USART2_SR) & (1 << 6)) == 0 || (ACCESS(USART2_SR) & (1 << 7)) == 0);  // Wait until transmission is complete
		ACCESS(USART2_DR) &= ~(0xFF); // Clear the data register
		ACCESS(USART2_DR) = *string; // Write the next character
		string++;  // Advance the pointer to the next character
	}
}
