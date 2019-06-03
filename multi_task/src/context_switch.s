/*********************************
* context_switch.s
*
* Note: OS context switch module
*
**********************************/

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

/* Note we allocated 256*4 = 1024 bytes of stack for each task*/

/* ARM assembly directives */

.syntax unified
.cpu cortex-m4
.thumb

/* ARM assembly definitions */


/* Routines from asm.h */

.global activate
.global initActivate
.global SVC_Handler

/***************************************************
 * initActivate
 *
 * Description: context switch routine for tasks
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
activate:

      cpsie i                                                                   /* enable IRQs to make the SVC call */

      /* save kernel state using msp */
      mrs ip, psr
    	push {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}

      /* retrieve routine parameters and switch to the process stack psp */
      ldmia r0!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}                     /* get control val and func address. r0 points to begining of an array address. r0! is used like a stack to pop */
      msr control, ip                                                           /* sp switched to process mode. toggling the bit control[1], we can switch between psp and msp */
      isb                                                                       /* When changing the stack pointer, software must use an ISB instruction immediately after the MSR instruction. */
      msr psp, r0                                                               /* sp now points to local function vars */

      /* software stack frame. load user state */


      /* hardware stack frame. the cpu pops r0-r3, r12 (IP), LR, PC, xPSR automatically */

      /* jump to user task*/
      bx lr


/***************************************************
 * SVC_Handler
 *
 * Description:
 *
 * Input:
 *
 * Output:
 *
 * Return:
 *
 * Note: supervisor call (handler mode)
 *
 *
 ***************************************************/
.thumb_func                                                                     /* indicate this exception is running in thumb state */
SVC_Handler:

      /* automatically use the msp as the sp when entering handler mode */

      /* save user state. Note: lr reg contains addr of calling fun */
      mrs r0, psp
      stmdb r0!, {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}                         /* save where task left off. "activate" will return r0 (stack pointer) */

      /* load kernel state */
      pop {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}                            /* load kernel state that was saved when we first entered activate */
      msr psr_nzcvq, ip

      /* back to the thread mode if no other active exception */
      bx lr                                                                     /* return into the function that called activate (calling fun ) */
