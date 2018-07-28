/*
 * task.c
 *
 */

/* submodule includes */

#include "task.h"

/* global vars */

PRIVATE struct task_t task_arr[MAX_TASK_NUMBER];

/* function definitions */

/***************************************************
 * task_init
 *
 * Description: initializes tasks states to "TASK_READY"
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
void task_init()
{
  for(int i = 0; i < MAX_TASK_NUMBER; i++)
  {
    task_arr[i].state = TASK_FREE;
  }
}

/***************************************************
 * task_spawn
 *
 * Description: creates new task
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

bool task_spawn()
{
  uint8_t taskSize = 0;

  while( taskSize <= MAX_TASK_NUMBER && task_arr[taskSize].state != TASK_FREE )
  {
    taskSize++;
  }

  if( taskSize >= MAX_TASK_NUMBER && task_arr[taskSize].state != TASK_FREE)
  {
      print("ERROR: ");

      return false;
  }

  /* manually allocate task stack */
  task_arr[taskSize].sp = (void*)(MAX_SP_ADDRESS - (MAX_SP_SIZE_BYTES*taskSize));

  /* populate stack frame with vals for debugging purposes */
  //task_arr[taskSize].sp =

  return true;
}

/***************************************************
 *
 * task_kill
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
void task_kill()
{

}
