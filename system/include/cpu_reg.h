/*
* cpu_reg.h
*
*/


/* submodule includes */

#include "common.h"

/* macro defines for cpu register and exception init/configs settings */

#define CPU_CCR                               ACCESS(0xE000ED14)
#define CPU_SHRS                              ACCESS(0xE000ED24)

/* mask macro defines */

#define  CCR_STKALIGN_8BYTE_MASK                          (1 << 9)              /* indicates stack alignment on exception entry mask */
#define  CCR_BFHFNMIGN_EN_MASK                            (1 << 8)              /* enables handlers with priority -1 or -2 to ignore data BusFaults caused by load and store instructions. This applies to the hard fault, NMI, and FAULTMASK escalated handlers */
#define  CCR_DIV_0_TRP_EN_MASK                            (1 << 4)              /* enables faulting or halting when the processor executes an SDIV or UDIV instruction with a divisor of 0 */
#define  CCR_UNALIGN_TRP_EN_MASK                          (1 << 3)              /* enables unaligned access traps mask */
#define  CCR_USERSETMPEND_EN_MASK                         (1 << 1)              /* enables unprivileged software access to the STIR, see Software Trigger Interrupt Register */
#define  CCR_THREAD_MODE_EXEC_RET_MASK                    (1 << 0)              /* processor can enter Thread mode from any level under the control of an EXC_RETURN value, see Exception return */
#define  CCR_THREAD_MODE_NO_EXCEPTION_ACTIVE_MASK        ~(1 << 0)              /* processor can enter Thread mode only when no exception is active */
#define  CCR_RESET_MASK                                   0x00000200            /* reset condition */

#define SHCSR_MEMFAULT_EN_MASK                            (1 << 16)             /* mem fault exception enable */
#define SHCSR_BUSFAULTENA_EN_MASK	                        (1 << 17)             /* bus fault exception enable */
#define	SHCSR_USGFAULTENA_EN_MASK	                        (1 << 18)             /* usage fault exception enable */
