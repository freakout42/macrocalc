/* $Id: mcutil.c,v 1.8 2002/02/28 13:08:28 axel Exp $
 */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <arx_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcelladr.h"
#include "mcfileio.h"
#include "mcutil.h"

void initvars (void)
/* Initializes various global variables */
{
int col, row;

for (row = 0; row <= lastrow; row++)
	for (col = 0; col <= lastcol; col++)
		deletecell (col, row);
markcol = markrow = omarkcol = omarkrow = windowrow = topbotrow =
	leftcol = toprow = curcol = currow = lastcol = lastrow = 0;
memset (colwidth, defaultwidth, sizeof(colwidth));
inithash();
} /* initvars */

void centercolstring (int col, char *colstr)
/* Changes a column to a centered string */
{
char	s[3];
int	space, spaces1, spaces2;

colstring (col, s);
spaces1 = ((int)colwidth[col] - (int)strlen(s)) / 2;
if (spaces1<0) spaces1 = 0;
spaces2 = colwidth[col] - strlen(s) - spaces1;
if (spaces2<0) spaces2 = 0;
space = colwidth[col]>strlen(s) ? strlen(s) : colwidth[col];
sprintf (colstr, "%*s%*.*s%*s", spaces1, "", space, space, s, spaces2, "");
} /* centercolstring */

int rowwidth (int row)
/* Returns the width in spaces of row */
{
return((row == 0) ? 1 : (int)log10(row + 1) + 1);
} /* rowwidth */

/* just to force linker to include lib_akey */
void nevercalled (void)
{
lib_akey ("000000", FALSE);
}
