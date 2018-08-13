

/* context_switch.s will simply load the task (function)
   address into the PC register for switching between
   kernel and user mode as well as switching tasks */

 /* MSP is main stack pointer and will be used at kernel and interrupt level
    PSP is processor stack pointer and will be used at user level tasks */

 /* MSP = 0xFFFFFFF9 (main stack) - Tells the handler to return using the MSP
    PSP = 0xFFFFFFFD (process stack) - Tells the handler to return using the PSP */

/* CONTROL[nPRIV] - bit 0; thread mode privileged vs unprivileged access
                    set bit 0 to 0 = privileged.
                    set bit 0 to 1 = unprivileged.
   CONTROL[SPSEL] - bit 1; sp selection (MSP vs PSP) */

/* CONTROL register can only be written when in privileged mode */
/* thread mode can be privileged or underprivileged
   handler mode can only be privileged */

/* Note we allocated 256*4 = 1024 bytes of stack for all tasks
   each task occupies 16 words (16*4 = 64 bytes of mem space on RAM) */


.syntax unified
.cpu cortex-m4
.thumb

/* Routines from asm.h */
.global activate
.global SVC_Handler

/* context switch routine (thread mode) */
activate:
      cpsie i                                                                   /* enable IRQs to make the SVC call */

      /* save kernel state into msp */
      mrs ip, psr
    	push {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}

      /* retrieve routine parameters and switch to the process stack psp */
      ldmfd r0!, {ip,lr}                                                        /* get control val and func address. r0 points to begining of an array address. r0! is used like a stack to pop */
      msr control, ip                                                           /* sp switched to process mode. toggling the bit control[1], we can switch between psp and msp */
      isb                                                                       /* When changing the stack pointer, software must use an ISB instruction immediately after the MSR instruction. his ensures that instructions after the ISB execute using the new stack pointer. See ISB. */
      msr psp, r0                                                               /* sp now points to local function vars */

      /* software stack frame. load user state */
      pop {r4-r11}                                                              /* software stack frame pop - contains routine local vars */

      /* hardware stack frame. the cpu pops r0-r3, r12 (IP), LR, PC, xPSR automatically */

      /* jump to user task*/
      bx lr

/* supervisor call (handler mode) */
.thumb_func
SVC_Handler:
      /* automatically use the msp as the sp when entering handler mode */

      /* save user state */
      mrs r0, psp
      stmdb r0!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}

      /* load kernel state */
      pop {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}
      msr psr_nzcvq, ip

      /* back to the thread mode if no other active exception */
      bx lr
