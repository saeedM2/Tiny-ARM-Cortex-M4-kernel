/*
 * task.c
 *
 */

/* submodule includes */

#include "task.h"

/* macro definitions */

#define ARM_STACK_POPS            16                                            /* unstacks 16 registers (r0 - r15) */
#define STACK_SIZE_WORDS          256                                           /* each task stack is 256 words (1024 bytes) in size */
#define SET_PSP_MASK              0x3                                           /* this mask switches msp to psp */
#define MAX_TASK_NUMBER           10                                            /* max number of tasks allowed on this kernel */

/* global vars */
PRIVATE struct task_t* task_arr[MAX_TASK_NUMBER];

/* enumerations */

enum task_state
{
  TASK_FREE,
  TASK_READY,
  TASK_RUNNING
};

/* type definitions */

struct task_t
{
  uint32_t* sp;
  enum task_state state;
};

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
    task_arr[i] = TASK_FREE;
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
void task_spawn()
{

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
