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
  print("SAEED1 testing new print function. Number: %d, Character: %c\n", 2, 'A');

  /* return control back to kernel */
  syscall();
}


void second(void)
{
  print("SAEED2 testing new print function. Number: %d, Character: %c\n", 2, 'A');
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
  struct task_t task = {.asp = NULL, .state = TASK_FREE };

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
  if( !taskReady(&task) )
  {
    return false;
  }
  print("SAEED3\n");
  /* start first task */
  activate(task.asp);

  print("SAEED4\n");

#if 0
task.asp = NULL;
task.state = TASK_FREE;
  /* task #2 */
  if ( !taskSpawn(&second) )
  {
      return false;
  }
  if( !taskReady(&task) )
  {
    return false;
  }

  activate(task.asp);
#endif

#if 0
  print("taskR = 0x%X , taskR.asp = 0x%X , taskR.state = %u\n", taskR, taskR.asp, taskR.state);
#endif

  /* intercept the current running task */
  if( !taskCurrRunning(&task) )
  {
      return false;
  }

	while(1); /* we can't exit */
	return 0;
}
