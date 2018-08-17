/*
 * asm.h
 *
 */

/* function declarations */

uint32_t* activate(uint32_t* task_stack);
void syscall(void);
int32_t fork(void);

#if 0
int currentTaskTCB; /* This var is of type tskTaskControlBlock */
void PendSVHandler(void);
#endif
