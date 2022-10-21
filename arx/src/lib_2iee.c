/* $Id: lib_2iee.c,v 1.11 2004/04/17 18:09:42 axel Exp $
 */

#include <math.h>
#include <unistd.h>

#if defined(i386) || defined(_I386) || defined(_M_IX86)

#elif defined(__hppa) || defined(sparc) || defined(_AIX)
static void swap (unsigned char *x1, unsigned char *x2)
{
unsigned char x;

x = *x1;
*x1 = *x2;
*x2 = x;
}

void *lib_swab (void *nc, int len)
{
unsigned char *p;
int c;

p = nc;
for (c=0; c<len/2; c++) swap (p+c, p+len-c-1);
return nc;
}

double lib_2iee (double d)
{
lib_swab (&d, sizeof(double));
return d;
}

double lib_iee2 (double d)
{
return lib_2iee(d);
}
#endif
