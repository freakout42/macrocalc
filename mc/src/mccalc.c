/* mccalc.c 1.7 2005/08/04 06:21:50 axel */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mc.h"
#include "mcell.h"
#include "mcparse.h"
#include "mcunit.h"
#include "mcfileio.h"
#include "mccalc.h"

CELLPTR recalcell (int col, int row)
/* Recalculates one single cell */
{
CELLPTR	cp;

cp = cell (col, row);
if (cp == NULL || !cpformula(cp)) return cp;
#ifdef DEBUG
fprintf (stderr, "recalcell1: %s -> %f\n", cptext(cp), cpvalue(cp));
#endif
origcol	= col;
origrow	= row;
value = parse (cptext(cp), NULL, unit, NULL);
#ifdef DEBUG
fprintf (stderr, "recalcell2: %s -> %f %s\n", cptext(cp), value, unit);
#endif
switch (cptype(cp))
 {
 case FORMULA:
	if (cpunit(cp)!=NULL) lcpvalue(cp) = unitconv (value, unit, cpunit(cp));
	else lcpvalue(cp) = value;
	break;
 case STRING:
	if (strlen(unit)>(size_t)cplength(cp))
		{
		lcpstring(cp) = realloc(cpstring(cp),strlen(unit)+1);
		lcplength(cp) = strlen(unit);
		}
	strcpy (lcpstring(cp), unit);
	break;
 }
#ifdef DEBUG
fprintf (stderr, "recalcell3: %s -> %f\n", cptext(cp), cpvalue(cp));
#endif
return cp;
} /* recalcell */

void recalcworksheet (void)
/* Recalculates all of the numbers in the speadsheet */
{
sheetwalk(recalcell);
} /* mcrecalc */
