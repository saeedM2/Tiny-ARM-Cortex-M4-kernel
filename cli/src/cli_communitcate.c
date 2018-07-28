/*
 * cli_communitcate.c
 *
 */

/* submodule includes */

#include "cli_communitcate.h"

/* function definitions */

/***************************************************
 * print
 *
 * Description:
 *
 * Input:
 *
 * Output:
 *
 * Return:
 *
 * Note: on a Unix system we must properly handle CR
 *       and LF. CR and LF might be handled differently
 *       depending on the serial monitor interface and
 *       the environment it runs on.
 ***************************************************/
void printUart(char* string)
{
  ACCESS(USART2_DR) &= ~(0xFF);

  /* Write CR before writing string to UART buf */
  writeUartBuf('\r');

  while(*string)
	{
    writeUartBuf(*string);
    string++;  // Advance the pointer to the next character
	}

  /* Write CR after done writing string to UART buf */
  writeUartBuf('\r');
}

/***************************************************
 * print
 *
 * Description:
 *
 * Input:
 *
 * Output:
 *
 * Return:
 *
 * Note:
 *
 ***************************************************/
 void print(char* formatBuf,...)
 {
   char str[MAX_STR_BUFFER];

   /* pt1. handle variable args */
   va_list varArgs;

  /* pt2. handle variable args */
   va_start(varArgs, formatBuf);

   /* format the string args */
   lvsnprintf(str, (MAX_STR_BUFFER - 1), formatBuf, varArgs);

   /* send formated string to UART send-buffer */
   printUart(str);

   /* pt3. handle variable args */
   va_end(varArgs);
 }

/***************************************************
 * read_uart
 *
 * Description:
 *
 * Input:
 *
 * Output:
 *
 * Return:
 *
 * Note:
 *
 *
 ***************************************************/
bool readUartBuf(char **buf)
{

  /* buf returns a string */

  return true;
}

/***************************************************
 * write_uart
 *
 * Description:
 *
 * Input:
 *
 * Output:
 *
 * Return:
 *
 * Note:
 *
 *
 ***************************************************/
 bool writeUartBuf(char c)
 {
   // See pg 548 of the ST RM0383 document for more info on the USART registers.
   while((ACCESS(USART2_SR) & (1 << 6)) == 0 || (ACCESS(USART2_SR) & (1 << 7)) == 0);  // Wait until transmission is complete
   ACCESS(USART2_DR) &= ~(0xFF); // Clear the data register
   ACCESS(USART2_DR) = c; // Write the next character

   return true;
 }
