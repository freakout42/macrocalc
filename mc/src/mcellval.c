/* $Id: mcellval.c,v 1.3 1997/04/05 15:47:34 axel Exp $
 */

#include <stdio.h>
#include <arx_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcellstr.h"
#include "mcellpar.h"
#include "mcmessag.h"

int valuecell (int col, int row)
/* Values a cell */
{
CELLPTR	cp = cell (col, row);
char	*strvalue;
int	color;

if (cp==NULL) return RET_SUCCESS;
switch (cptype(cp)) {
 case CONSTANT:
 case FORMULA:
 case STRING:
	strvalue = cellstring (col, row, &color, FVALUE);
	if ((cp = parsecell(strvalue, col, row)) == NULL) 
		errormsg (MSGLOMEM);
 }
return RET_SUCCESS;
} /* valuecell */
