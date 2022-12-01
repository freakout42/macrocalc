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
cellr cr;

cp = cell (col, row);
if (cp == NULL || !cpformula(cp)) return cp;
#ifdef DEBUG
fprintf (stderr, "recalcell1: %s -> %f\n", cptext(cp), cpvalue(cp));
#endif
cr = *cp;
parse(&cr, NULL);
#ifdef DEBUG
fprintf (stderr, "recalcell2: %s -> %f %s\n", cptext(&cr), cpvalue(&cr), cpunit(&cr));
#endif
migratcell(cp, &cr);
return cp;
} /* recalcell */

void recalcworksheet (void)
/* Recalculates all of the numbers in the speadsheet */
{
sheetwalk(recalcell);
} /* mcrecalc */
