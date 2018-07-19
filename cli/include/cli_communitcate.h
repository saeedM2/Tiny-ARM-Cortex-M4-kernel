/*
 * cli_communitcate.h
 *
 */

/* submodule includes */

#include "uart_init.h"

/* function declarations */

void print(char *string);
bool read_uart(char **buf);
bool write_uart(char c);
