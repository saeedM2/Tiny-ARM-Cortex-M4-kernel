/*
 * main.c
 *
 */

#include "system_init.h"
#include "cli_communitcate.h"
#include "asm.h"

void first(void)
{
	//char *string = "SAEED Hello, World!\n\r";

	print("3aeed\n");
	while(1);
}

/***************************************************
 * main
 *
 ***************************************************/
int main(void) {

	initialize_core();

	/* tasks are 256*4 bytes in size */
	unsigned int stack_1[256];
	unsigned int *stack_1_start = stack_1 + 256 - 16;   /* place activate params at end of stack element: (256 - 16) */
	stack_1_start[0] = 3;                               /* control register param to transition into psp stack */
  stack_1_start[1] = (unsigned int)&first;            /* func address parameter */

	/* transition from kernel mode to user mode */
	print("3Starting\n");

	/* kernel switches to next active task context */
	/* kernel gives control to task  */
	activate(stack_1_start);

	while(1); /* we can't exit, there's nowhere to go */
	return 0;
}
