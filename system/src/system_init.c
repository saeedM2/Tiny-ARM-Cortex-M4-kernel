/*
 * system_init.c
 *
 */

/* submodule includes */

#include "system_init.h"

/* function definitions */

/***************************************************
 * initialize_core
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
void initializeCore(void)
{
    /* system handler priority registers initializing svc call priority */
    ACCESS(SHPR2) &= (SVC_SH_PRI0_MASK);

    /* system irq priority register */
    ACCESS(IPR2) &= (SVC_NVIC_PRI0_MASK);

    /* initialize uart peripheral for cli communication */
  	InitUART();
}
