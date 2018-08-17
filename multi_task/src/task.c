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
    task_arr[i].sp = NULL;
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
 * Note: due to how the context switch routines
 *       were designed we changed the order of
 *       registers by pointed by the ".sp", see below
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
  task_arr[taskSize].sp = (void*)(MAX_SP_ADDRESS - (MAX_SP_SIZE_WORDS*taskSize));  /* managing stack space */

  /* setup initial stack frame for debugging purposes */

  /* hardware stack (saved by cpu automatically) */
  *(task_arr[taskSize].sp--) = 15;                                /* xPSR */
  *(task_arr[taskSize].sp--) = 14;                                /* PC */
  *(task_arr[taskSize].sp--) = 3;                                /* R3 */
  *(task_arr[taskSize].sp--) = 2;                                /* R2 */
  *(task_arr[taskSize].sp--) = 1;                                /* R1 */
  *(task_arr[taskSize].sp--) = 0;                                /* R0 */

  /* parameters passed to context switch routine (activate) */
  *(task_arr[taskSize].sp--) = (uint32_t)fcnp;                                 /* LR */
  *(task_arr[taskSize].sp--) = SET_PSP_MASK;                                   /* IP */

  /* software stack (manually stacked and popped during context switch) */
  *(task_arr[taskSize].sp--) = 11;                                /* R11 */
  *(task_arr[taskSize].sp--) = 10;                               /* R10 */
  *(task_arr[taskSize].sp--) = 9;                                /* R9 */
  *(task_arr[taskSize].sp--) = 8;                                /* R8 */
  *(task_arr[taskSize].sp--) = 6;                                /* R6 */
  *(task_arr[taskSize].sp--) = 7;                                /* R7 */
  *(task_arr[taskSize].sp--) = 5;                               /* R5 */
  *(task_arr[taskSize].sp) = 4;                               /* R4 */

  /* note lr is also automatically saved by cpu during context switch but
     we sometimes change lr manually, like when first call a task  */

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

      taskSize++;
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
