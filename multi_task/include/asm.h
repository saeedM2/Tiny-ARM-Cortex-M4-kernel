/*
 * asm.h
 *
 */

/* function declarations */

void activate(uint32_t* task_stack);
void syscall(void);

#if 0
int currentTaskTCB; /* This var is of type tskTaskControlBlock */
void PendSVHandler(void);
#endif
