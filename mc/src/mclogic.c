/* $Id: mclogic.c,v 1.3 1995/02/28 15:08:08 axel Rel $
 */

#define _XOPEN_SOURCE
#include <math.h>
#include <float.h>

double mcif (double l, double t, double f)

{
return fabs(l) > DBL_MIN ? t : f;
}
