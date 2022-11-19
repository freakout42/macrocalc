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
int typ;

cp = cell (col, row);
if (cp == NULL || !cpformula(cp)) return cp;
#ifdef DEBUG
fprintf (stderr, "recalcell1: %s -> %f\n", cptext(cp), cpvalue(cp));
#endif
origcol	= col;
origrow	= row;
typ = parse (cp, NULL, NULL);
#ifdef DEBUG
fprintf (stderr, "recalcell2: %s -> %f %s\n", cptext(cp), cpvalue(cp), cpunit(cp));
#endif
#ifdef NOMORENEEDED
switch (cptype(cp))
 {
 case FORMULA:
	cpvalue(cp) = (cpunit(cp)!=NULL) ? unitconv (value, unit, cpunit(cp)) : value;
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
#endif
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
