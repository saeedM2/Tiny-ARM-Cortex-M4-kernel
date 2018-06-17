/*
 * main.c
 *
 */

#include <stdio.h>

#include "system_init.h"


/***************************************************
 * main
 *
 ***************************************************/
int main(void) {

	initialize_core();

	char *string = "SAEED Hello, World!\n";
	while(*string) {
		while((ACCESS(USART2_SR) & (1 << 6)) == 0 || (ACCESS(USART2_SR) & (1 << 7)) == 0);  // Wait until transmission is
		*(volatile unsigned int *)(USART2_DR) = ~(0xFF);
		*(volatile unsigned int *)(USART2_DR) = *string;
		string++;
	}

	while(1); /* We can't exit, there's nowhere to go */
	return 0;
}
