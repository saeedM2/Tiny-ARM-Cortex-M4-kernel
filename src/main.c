/*
 * main.c
 *
 */

#include <stdio.h>

#include "system_init.h"
#include "cli_print.h"


/***************************************************
 * main
 *
 ***************************************************/
int main(void) {

	initialize_core();

	char *string = "SAEED Hello, World!\r\n";

	print(string);

	while(1); /* We can't exit, there's nowhere to go */
	return 0;
}
