/*
 * common.h
 *
 */

#ifndef COMMON_H
#define COMMON_H

 /* standard includes */

 #include <stdbool.h>
 #include <stdio.h>
 #include <stdint.h>
 #include <stdarg.h>
 #include <stdlib.h>

/* submodule includes */
#include "cli_communitcate.h"
#include "lprintf.h"

/* macro function definitions */

/***************************************************
 * ACCESS
 *
 * Description:   macro function for setting address of register
 *
 ***************************************************/
#define ACCESS(address)             *((volatile unsigned int*)(address))

/* macro definitions */

#define PRIVATE                     static      /* contain scope of function and vars to a particular file */



#endif /* end of header gaurds */
