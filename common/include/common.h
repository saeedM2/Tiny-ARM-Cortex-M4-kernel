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
#include "lsprintf.h"

/* macro function definitions */

/***************************************************
 * ACCESS
 *
 * Description:   macro function accessing peripheral address
 *                safely using volatile
 *
 ***************************************************/
#define ACCESS(address)             *((volatile unsigned int*)(address))

/* macro definitions */

#define PRIVATE                     static      /* impose scope of function and vars to a particular file */



#endif /* end of header gaurds */
