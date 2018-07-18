/*
 * cli_communitcate.c
 *
 */

#include "cli_communitcate.h"

/***************************************************
 * print
 *
 * Note: on a Unix system we must properly handle CR
 *       and LF. CR and LF might be handled differently
 *       depending on the serial monitor interface and
 *       the environment it runs on.
 ***************************************************/
void print(char *string)
{
  ACCESS(USART2_DR) &= ~(0xFF);

  /* Write CR before writing string to UART buf */
  write_uart('\r');

  while(*string)
	{
    write_uart(*string);
    string++;  // Advance the pointer to the next character
	}

  /* Write CR after done writing string to UART buf */
  write_uart('\r');
}

/***************************************************
 * read_uart
 *
 ***************************************************/
bool read_uart(char **buf)
{

  /* buf returns a string */

  return true;
}

/***************************************************
 * write_uart
 *
 ***************************************************/
 bool write_uart(char c)
 {
   // See pg 548 of the ST RM0383 document for more info on the USART registers.
   while((ACCESS(USART2_SR) & (1 << 6)) == 0 || (ACCESS(USART2_SR) & (1 << 7)) == 0);  // Wait until transmission is complete
   ACCESS(USART2_DR) &= ~(0xFF); // Clear the data register
   ACCESS(USART2_DR) = c; // Write the next character

   return true;
 }
