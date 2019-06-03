
/* submodule includes */

#include "common.h"

/* macro definitions */

#define SET_PSP_MASK              0x2                                           /* this mask switches msp to psp. Privileged thread mode with psp stack */
#define PSR_DEFAULT_MASK          0x21000000                                    /* see ARM cortex-M data sheet for register details*/
#define MAX_TASK_NUMBER           10                                            /* max number of tasks allowed on this kernel */
#define MAX_SP_ADDRESS            0x20001000                                    /* end of the RAM */
#define MAX_SP_SIZE_WORDS         256                                           /* each task stack is 256 words in size (256*4 = 1024 bytes) */
#define INIT_SP_TASK_ADDRESS      (MAX_SP_ADDRESS - MAX_SP_SIZE_BYTES)          /* address of the first stack pointer */
#define MAIN_RETURN               0xFFFFFFF9                                    /* Tells the handler to return using the MSP */
#define THREAD_RETURN             0xFFFFFFFD                                    /* Tells the handler to return using the PSP */

/* global vars */

/* enumeration definitions */

enum task_state
{
  TASK_FREE,
  TASK_READY,
  TASK_RUNNING,
};

/* struct definitions */

struct task_t
{
  uint32_t* sp;                                                                 /* virtual task stack pointer */
  enum task_state state;                                                        /* state of a given task (free to be allocated, ready to run, currenlty running)*/
};

/* function declarations */

void taskInit();
bool taskSpawn(void (*fcnp)(void));
void taskKill();
bool taskReady(struct task_t* taskReady);
bool taskCurrRunning(struct task_t* currTask);
