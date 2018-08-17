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

void task2(void)
{
  print("task2: in user mode 1\n");

  syscall();

  print("task2: in user mode 2\n");

  /* dont exit task */
  print("task2: dont exit task...\n");
  while(1)
  {
    syscall();
  }

}

void task1(void)
{
  print("task1: in user mode 1\n");

  /* return control back to kernel */
  if( fork() == 0 )
  {
    task2();
  }

  print("task1: in user mode 2\n");

  /* dont exit task */
  print("task1: dont exit task...\n");
  while(1)
  {
    syscall();
  }

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
  struct task_t task_struc = {.sp = NULL, .state = TASK_FREE };

  /* initialize core services */
	initializeCore();

  /* initialize task table */
  taskInit();

  /* create first task */
  if ( !taskSpawn(&task1) )
  {
      return false;
  }
  /* ready the task to run */
  if( !taskReady(&task_struc) )
  {
    return false;
  }

  print("task1:\n");
  task_struc.sp = activate(task_struc.sp);

  print("task1: re-entering...\n");

  task_struc.sp = activate(task_struc.sp);

  print("Done1\n");

  /* task2 */
  if ( !taskSpawn(&task2) )
  {
      return false;
  }
  /* ready the task to run */
  if( !taskReady(&task_struc) )
  {
    return false;
  }

  print("task2:\n");
  task_struc.sp = activate(task_struc.sp);

  print("task2: re-entering...\n");

  task_struc.sp = activate(task_struc.sp);

  print("Done2\n");

#if 0
task.sp = NULL;
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

  activate(task.sp);
#endif

#if 0
  print("taskR = 0x%X , taskR.sp = 0x%X , taskR.state = %u\n", taskR, taskR.sp, taskR.state);
#endif

  /* intercept the current running task */
  if( !taskCurrRunning(&task_struc) )
  {
      return false;
  }

	while(1); /* we can't exit */
	return 0;
}
