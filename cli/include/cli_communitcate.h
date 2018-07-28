/*
 * cli_communitcate.h
 *
 */

/* submodule includes */

#include "uart_init.h"

/* macro definitions */

#define MAX_STR_BUFFER      1024

/* function declarations */

void print(char* formatBuf, ...);
void printUart(char *string);
bool readUartBuf(char **buf);
bool writeUartBuf(char c);
