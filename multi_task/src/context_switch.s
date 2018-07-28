

/* context_switch.s will simply load the task (function)
   address into the PC register for switching between
   kernel and user mode as well as switching tasks */

 /* MSP is main stack pointer and will be used at kernel and interrupt level
    PSP is processor stack pointer and will be used at user level tasks */

 /* MSP = 0xFFFFFFF9 (main stack)
    PSP = 0xFFFFFFFD (process stack) */

/* Note we allocated 256*4 = 1024 bytes of stack for each task */

.syntax unified
.cpu cortex-m4
.thumb

/* Routines from asm.h */
.global activate
.global currentTaskTCB
.global PendSVHandler

/* context switch routine */
activate:
      /* switch to the process stack */
      ldmfd r0!, {ip,lr}                                                        /* get control val and func address. r0 points to begining of an array address. r0! is used like a stack to pop */
      msr psp, r0                                                               /* function pointer address. The stack pointer points to the initial memory location of our created task stack */
      msr control, ip                                                           /* switch to process mode. Using control[1] we can switch between psp and msp and also control privilege level  */

      /* software stack frame. load user state */
      pop {r4-r11}                                                              /* software stack frame pop - contains routine local vars */

      /* hardware stack frame. the cpu pops r0-r3, r12 (IP), LR, PC, xPSR automatically */

      /* jump to user task*/
      bx lr
