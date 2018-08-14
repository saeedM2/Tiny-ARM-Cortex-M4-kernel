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
    /* initialize uart peripheral for cli communication */
  	InitUART();
}
