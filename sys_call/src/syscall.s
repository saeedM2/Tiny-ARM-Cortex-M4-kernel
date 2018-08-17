/*
 * syscall.s
 *
 * Note: system call wrapper
 *
 */

/* submodule includes */

.global syscall
.global fork

/***************************************************
 * syscall
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
syscall:
        push {r7}
        mov r7, #0x0
        svc 0                                                                   /* invokes the SVC_Handler in context_switch.s */
        pop {r7}
        bx lr

/***************************************************
 * fork
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
fork:
      push {r7}
      mov r7, #0x1
      svc 0
      pop {r7}
      bx lr
