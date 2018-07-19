/*
 * common.h
 *
 */

 /* standard includes */

 #include <stdbool.h>
 #include <stdio.h>
 #include <stdint.h>

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
