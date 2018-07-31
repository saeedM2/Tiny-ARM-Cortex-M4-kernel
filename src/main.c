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
  struct task_t taskR;

  /* initialize core services */
	initializeCore();

  /* initialize task table */
  taskInit();

  /* create first task */
  if ( !taskSpawn(&first) )
  {
      return false;
  }

  /* ready the task to run */
  if( !taskReady(&taskR) )
  {
    return false;
  }

  print("SAEED1\n");
  activate(taskR.asp);

	while(1); /* we can't exit */
	return 0;
}
