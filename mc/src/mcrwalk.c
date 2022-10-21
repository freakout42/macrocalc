/* $Id: mcrwalk.c,v 1.1 1999/10/24 05:59:08 axel Exp $
 */

#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <arx_def.h>
#include "mc.h"
#include "mcutil.h"
#include "mcmisc.h"
#include "mcell.h"
#include "mcelladr.h"

void rangewalk (int (*fcn)(int col, int row),
		struct CELLADR cell1, struct CELLADR cell2)
/* walks through cellrange */
{
int	col, row;

if (cell1.col > cell2.col) swap (&cell1.col, &cell2.col);
if (cell1.row > cell2.row) swap (&cell1.row, &cell2.row);
for (col = cell1.col; col <= cell2.col; col++)
	for (row = cell1.row; row <= cell2.row; row++)
		if (cell (col, row)) (*fcn) (col, row);
return;
}

void rangevwalk (int (*fcn)(struct CELLVALUE value),
		struct CELLADR cell1, struct CELLADR cell2)
/* walks through cellrange */
{
int	col, row;
CELLPTR	cp;

if (cell1.col > cell2.col) swap(&cell1.col, &cell2.col);
if (cell1.row > cell2.row) swap(&cell1.row, &cell2.row);
for (col = cell1.col; col <= cell2.col; col++)
	for (row = cell1.row; row <= cell2.row; row++)
		if ((cp = cell (col, row)) != NULL && cpnumber(cp))
			(*fcn) (cpval(cp));
return;
}
