/*
 * lmemcpy.c
 *
 */

/* submodule includes */
#include "lmemcpy.h"

/* function definitions */

/***************************************************
 * lmemcpy
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
void* lmemcpy(void *dest, const void *src, size_t n)
{
    char *d = dest;
    const char *s = src;
    size_t i;

    /* copy memory */
    for(i = 0; i < n; i++)
    {
      d[i] = s[i];
    }

    return d;
}
