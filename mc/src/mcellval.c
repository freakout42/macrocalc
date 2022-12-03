/* mcellval.c 1.3 1997/04/05 15:47:34 axel */

#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcellstr.h"
#include "mcmessag.h"

int valuecell (int col, int row) {
/* Values a cell */
CELLPTR cp;
cellr cr;
char *strvalue;
int color;

if ((cp = cell(col, row)) == NULL) return RET_SUCCESS;
switch (cptype(cp)) {
 case CONSTANT:
 case FORMULA:
 case STRING:
  memset(&cr, 0, sizeof(cellr));
  cpcol(&cr) = col;
  cprow(&cr) = row;
  cptype(&cr) = TEXT;
  strvalue = cellstring (col, row, &color, FVALUE);
  *--strvalue = STRLEFT;
  cptext(&cr) = strvalue;
  if ((cp = migratcell(cp, &cr)) == NULL) errormsg (MSGLOMEM);
  changed = TRUE;
}
return RET_SUCCESS;
} /* valuecell */
