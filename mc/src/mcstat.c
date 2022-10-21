/* $Id: mcstat.c,v 1.4 1999/10/24 05:58:08 axel Exp $
 */

#include <stddef.h>
#include <arx_def.h>
#include <math.h>
#include "mcunit.h"
#include "mc.h"
#include "mcrwalk.h"

static int	n;
static double	value, qvalue, maxvalue, minvalue;
extern char	*yyunit;

void statinit (void)
{
n = 0;
value = qvalue = 0.;
yyunit = NULL;
}

int statadd (struct CELLVALUE val)
{
double	v;

if (n++ == 0)	yyunit	= val.unit;
v = unitconv (val.value, val.unit, yyunit);
value	+= v;
qvalue	+= v*v;
if (n == 1 || v > maxvalue) maxvalue = v;
if (n == 1 || v < minvalue) minvalue = v;
return RET_SUCCESS;
}

double avg (struct CELLADR cell1, struct CELLADR cell2)

/* Finds the value of cellrange average */
{
statinit();
rangevwalk (statadd, cell1, cell2);
return value/(double)n;
}

double count (struct CELLADR cell1, struct CELLADR cell2)
/* Finds the value of cellrange count */
{
statinit();
rangevwalk (statadd, cell1, cell2);
return (double)n;
}

double maximum (struct CELLADR cell1, struct CELLADR cell2)
/* Finds the value of cellrange max */
{
statinit();
rangevwalk (statadd, cell1, cell2);
return maxvalue;
}

double minimum (struct CELLADR cell1, struct CELLADR cell2)
/* Finds the value of cellrange min */
{
statinit();
rangevwalk (statadd, cell1, cell2);
return minvalue;
}

double var (struct CELLADR cell1, struct CELLADR cell2)
/* Finds the value of cellrange variance */
{
statinit();
rangevwalk (statadd, cell1, cell2);
return (qvalue-value*value/(double)n)/(double)n;
}

double std (struct CELLADR cell1, struct CELLADR cell2)
/* Finds the value of cellrange std */
{
return sqrt (var (cell1, cell2));
}

double sum (struct CELLADR cell1, struct CELLADR cell2)
/* Finds the value of cellrange sum */
{
statinit();
rangevwalk (statadd, cell1, cell2);
return value;
}
