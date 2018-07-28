/*
 * main.c
 *
 * Note: mini kernel for ARM Cortex M4 (32bit arch) little endian chip
 *
 */

/* submodule includes */

#include "system_init.h"
#include "cli_communitcate.h"
#include "asm.h"
#include "task.h"

void first(void)
{
  print("SAEED testing new print function. Number: %d, Character: %c\n", 2, 'A');
	while(1);
}

/***************************************************
 * main
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
int main(void)
{
	initialize_core();

	/* tasks are 256*4 bytes in size */
	uint32_t stack_1[STACK_SIZE_WORDS];
	uint32_t *stack_1_start = stack_1 + STACK_SIZE_WORDS - ARM_STACK_POPS;        /* place activate params near end of task stack: (256 - 16). */
	stack_1_start[0] = 3;                                                         /* control register param to transition into psp stack */
  stack_1_start[1] = (uint32_t)&first;                                          /* func address parameter */

	/* kernel switches to next active task context */
	/* kernel gives control to task  */
	activate(stack_1_start);

	while(1); /* we can't exit */
	return 0;
}
