/*
 * task.c
 *
 */

/* submodule includes */

#include "task.h"

/* global vars */

PRIVATE struct task_t task_arr[MAX_TASK_NUMBER];                                /* array of task objects supported by the kernel */

/* function definitions */

/***************************************************
 * Name: task_init
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
void taskInit()
{
  for(int i = 0; i <= MAX_TASK_NUMBER; i++)
  {
    task_arr[i].asp = NULL;
    task_arr[i].state = TASK_FREE;
  }
}

/***************************************************
 * Name: task_spawn
 *
 * Description: creates new task.
 *
 * Input:
 *
 * Output:
 *
 * Return:
 *
 * Note: the stack grows downwards for the ARM CORTEX-M arch
 *       hence "*.sp--" is used in this func definition. The
 *       PSP control register mask value is at top of the stack
 *       when passed to activate task routine.
 *
 ***************************************************/

bool taskSpawn(void (*fcnp)(void))
{
  uint8_t taskSize = 0;

  /* find a free task */
  while( taskSize <= MAX_TASK_NUMBER && task_arr[taskSize].state != TASK_FREE )
  {
    taskSize++;
  }

  /* handle no free task available */
  if( taskSize >= MAX_TASK_NUMBER && task_arr[taskSize].state != TASK_FREE)
  {
      print("ERROR: cannot create new task because max alloted tasks has been reached\ncurrent tasks active = %d, max tasks allowed = %u , task_arr[taskSize].state = %d\n",
             taskSize, MAX_TASK_NUMBER,task_arr[taskSize].state);

      return false;
  }

  /* manually allocate virtual asending stack for each task */
  task_arr[taskSize].asp = (void*)(MAX_SP_ADDRESS - (MAX_SP_SIZE_WORDS*taskSize));  /* managing stack space */

  /* setup initial stack frame for debugging purposes */

  /* software stack (manually stacked and popped during context switch) */
  *(task_arr[taskSize].asp--) = 4;                                /* R4 */
  *(task_arr[taskSize].asp--) = 5;                                /* R5 */
  *(task_arr[taskSize].asp--) = 6;                                /* R6 */
  *(task_arr[taskSize].asp--) = 7;                                /* R7 */
  *(task_arr[taskSize].asp--) = 8;                                /* R8 */
  *(task_arr[taskSize].asp--) = 9;                                /* R9 */
  *(task_arr[taskSize].asp--) = 10;                               /* R10 */
  *(task_arr[taskSize].asp--) = 11;                               /* R11 */

  /* hardware stack (saved by cpu automatically) */
  *(task_arr[taskSize].asp--) = 0;                                /* R0 */
  *(task_arr[taskSize].asp--) = 1;                                /* R1 */
  *(task_arr[taskSize].asp--) = 2;                                /* R2 */
  *(task_arr[taskSize].asp--) = 3;                                /* R3 */
  *(task_arr[taskSize].asp--) = 12;                               /* R12 (IP) */
  *(task_arr[taskSize].asp--) = 0;                                /* LR */

  /* parameters passed to context switch routine (activate) */
  *(task_arr[taskSize].asp--) = (uint32_t)fcnp;                                 /* SP to function pointer */
  *(task_arr[taskSize].asp) = SET_PSP_MASK;                                     /* PSP control register mask bits */

  task_arr[taskSize].state = TASK_READY;

  return true;
}

/***************************************************
 *
 * Name: taskKill
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
void taskKill()
{

}

/***************************************************
 *
 * Name: taskReady
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
bool taskReady(struct task_t* taskReady)
{
    int8_t taskSize = 0;

    while(taskSize <= MAX_TASK_NUMBER)
    {
      if(task_arr[taskSize].state == TASK_READY)
      {
        task_arr[taskSize].state = TASK_RUNNING;
        *taskReady = task_arr[taskSize];

        return true;
      }
    }

    if(taskSize >= MAX_TASK_NUMBER && task_arr[taskSize].state != TASK_READY)
    {
      print("ERROR: no task ready to run.\ncurrent task number = %u, max allowed tasks = %u, task_arr[taskSize].state = %d\n",
             taskSize, MAX_TASK_NUMBER, task_arr[taskSize].state);

      return false;
    }

    /* default state*/
    return false;
}

/***************************************************
 *
 * Name: taskCurrRunning
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
 bool taskCurrRunning(struct task_t* currTask)
 {
   int8_t taskSize = 0;

   /* find the current task running */
   while(taskSize <= MAX_TASK_NUMBER)
   {
     if(task_arr[taskSize].state == TASK_RUNNING)
     {
       *currTask = task_arr[taskSize];

       return true;
     }

      taskSize++;
   }

   if(taskSize >= MAX_TASK_NUMBER && task_arr[taskSize].state != TASK_RUNNING)
   {
     print("ERROR: no running task detected.\ncurrent task number = %u, max allowed tasks = %u, task_arr[taskSize].state = %d\n",
           taskSize, MAX_TASK_NUMBER, task_arr[taskSize].state);

    return false;
   }

   return false;
 }
