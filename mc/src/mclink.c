/* $Id: mclink.c,v 1.5 1997/01/23 19:08:09 axel Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <arx_def.h>
#include "mc.h"
#include "mcelladr.h"
#include "mclink.h"

double mclink (char *sf, int col, int row)
/* link spreadsheets */
{
FILE	*p;
char	buf[MAXINPUT+2];
char	adr[MAXADR];

celladrstring (col, row, adr);
sprintf (buf, "%s/bin/mcellval %s %s", libpath, sf, adr);
if (	!(p = popen (buf, "r"))
   ||	fgets (buf, sizeof(buf), p) == NULL
   ||	pclose (p)	)
	return HUGE_VAL;
return atof(buf);
} /* mclink */
