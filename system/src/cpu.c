/*
* cpu.c
*
*/

/* submodule includes */

#include "cpu.h"

/* function definitions */

/***************************************************
*
* Name: cpuInit
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
void cpuInit(void)
{
  /* setup system exceptions: catch divide by 0 and unaligned access */
  CPU_CCR |= CCR_UNALIGN_TRP_EN_MASK | CCR_DIV_0_TRP_EN_MASK;

  /* enable reporting of memory faults, bus faults and usage faults */
  CPU_SHRS |= SHCSR_MEMFAULT_EN_MASK | SHCSR_BUSFAULTENA_EN_MASK |
              SHCSR_USGFAULTENA_EN_MASK;              
}
