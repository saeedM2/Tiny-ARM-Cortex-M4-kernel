/*
 * uart_init.c
 *
 */

/* submodule includes */

#include "uart_init.h"

/* function definitions */

/***************************************************
 * InitUART
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
void InitUART()
{
	/* Give a clock to port A as we'll be using one of its pins for transfer of data.*/
	ACCESS(RCC_AHB1ENR) |= 1;

	ACCESS(GPIOA_MODER) |= ((1 << 5) | (1 << 7));

	/* USART2 is AF7.  And pg 160 of the same document */
	ACCESS(GPIOA_AFRL) |= ((7 << 8) | (7 << 12));

	/* Give a clock USART2. */
	ACCESS(RCC_APB1ENR) |= (1 << 17);

	ACCESS(USART2_BRR) |= 0x683;

	/* Enable USART2 for transmitting and receiving data. */
	ACCESS(USART2_CR1) |= ((1 << 2) | (1 << 3) | (1 << 5) | (1 << 13));
}
