/*
 * syscall.s
 *
 * Note: system call wrapper
 *
 */

/* submodule includes */
.global syscall

syscall:
        svc 0                                                                   /* invokes the SVC_Handler in context_switch.s */
        bx lr
