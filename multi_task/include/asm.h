/*
 * asm.h
 *
 */

/* function declarations */

uint32_t* initActivate(uint32_t* task_stack);
uint32_t* activate(uint32_t* task_stack);
void syscall(void);

#if 0
int currentTaskTCB; /* This var is of type tskTaskControlBlock */
void PendSVHandler(void);
#endif
