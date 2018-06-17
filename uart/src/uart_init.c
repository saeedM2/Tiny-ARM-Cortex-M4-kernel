/*
 * uart_init.c
 *
 */


#include "uart_init.h"

/***************************************************
 * InitUART
 *
 ***************************************************/
void InitUART()
{
	// Give a clock to port A as we'll be using one of its pins for transfer of data.
	ACCESS(RCC_AHB1ENR) |= 1;

	// See pg 19 of the ST UM1842 document.  We'll be using USART2.  USART2 TX occurs on
	// PA2 and USART2 RX occurs on PA3 so we set this pin to alternate mode.
	ACCESS(GPIOA_MODER) |= ((1 << 5) | (1 << 7));

	// See pg 149 of ST RM0383 document.  USART2 is AF7.  And pg 160 of the same document
	// shows alternate function for pins 2 and 3 are set using alternate function low register
	// bits 8-thru-11.
	ACCESS(GPIOA_AFRL) |= ((7 << 8) | (7 << 12));


	// Give a clock USART2.  See pg 117.
	ACCESS(RCC_APB1ENR) |= (1 << 17);

	// Here we set the baud rate.  This is explained on 519 of ST RM0383.  The peripheral
	// clock is 16 MHz by default.  So, the calculation for the USARTDIV is:
	// DesiredBaudRate = 16MHz / 16*USARTDIV
	// Note that the fractional part of USARTDIV is represented with only 4 bits.  So
	// if we use 9600 this will result in a small error of 0.02% (see pg 522).  Therefore
	// the baud rate is actually 9,598.  Again, see pg 522.
	ACCESS(USART2_BRR) |= 0x683;

	// Enable USART2 for transmitting and receiving data.  Receiving data is done through
	// an interrupt.  This is specified using by setting bit 5 (RXNE interrupt enable).
	ACCESS(USART2_CR1) |= ((1 << 2) | (1 << 3) | (1 << 5) | (1 << 13));
}
