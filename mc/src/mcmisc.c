/* $Id: mcmisc.c,v 1.2 2004/04/16 19:30:30 axel Exp $
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arx_def.h>
#include "mc.h"
#include "mcmisc.h"

void fatal (int x, char *s)
{
fprintf(stderr, "%s: %s\n", progname, s);
exit(x);
}

void swap (int *val1, int *val2)
/* Swaps the first and second values */
{
int	temp;

temp	= *val1;
*val1	= *val2;
*val2	= temp;
} /* swap */

void rmendnl (char *s)
{
char	*send;

send	= s + strlen(s) - 1;
if (*send == '\n') *send = '\0';
}
