/* $Id: mcpform.c,v 1.2 1997/04/07 18:07:59 axel Exp $
 */

#include <stdio.h>
#include "mc.h"
#include "mcell.h"
#include "mcommon.h"
#include "mcfileio.h"
#include "mcpform.h"

void printformula (FILE *file)
/* Prints formulas of a spreadsheet range to a file */
{
CELLPTR	cp;
int	col, row;

for (row = omarkrow; row <= markrow; row++) {
	for (col = omarkcol; col <= markcol; col++) {
		fprintf(file, "%s%c",
			(cp = cell(col, row)) ? (cptype(cp)==UNITT?"":cptext(cp)) : "",
			col==markcol ? '\n' : '\t' );
		}
	}
}
