/* mcfileio.c 1.10 2004/07/04 04:03:31 axel */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcmessag.h"
#include "mcellact.h"
#include "mcellstr.h"
#include "mcelladr.h"
#include "mcfilef.h"
#include "mcrange.h"
#include "mcolor.h"
#include "mcfileio.h"

int lastcol, lastrow, markcol=0, markrow=0, omarkcol=0, omarkrow=0;

int loadfile (FILE *file)
/* Loads a new spreadsheet */
{
int	reallastcol = lastcol, reallastrow = lastrow;
char	cols[8];
char	line[MAXINPUT+1];
int	col, row;
cellr cp;
CELLPTR ct, cu;
int	att;
unsigned char abits;
int	form;
char	tex[MAXINPUT+2];
double	val;
char newold;

str_gets (file, line, sizeof(line));
if (strncmp(line, colnames, 4) != 0) return RET_FATAL;
newold = line[4];
str_gets (file, line, sizeof(line));
if (strspn(line, "-\t") != strlen(line)) return RET_FATAL;
while (fscanf (file, "%s\t%d\t%d\t%lf\t", cols, &att, &form, &val)==4)
	{
	str_gets (file, tex+1, MAXINPUT);
	celladr (cols, &col, &row);
	if (!strcmp (tex+1, "windowdef"))
		{
		curcol		= col;
		currow		= row;
		windowrow	= att;
		toprow		= form;
		windowline	= borderline + (int)val;
		}
	else if (!strcmp (tex+1, "coldef"))
		{
		celladr (cols, &col, &row);
		colwidth[col]	= (unsigned char) form;
		}
	else if (!strncmp (tex+1, "rangedef:", 9))
		{
		updaterange (tex+10, col, row, att, form);
		}
	else
		{
		if (newold == '\t') {
			abits = att;
			abits &= TYPEM;
			if (abits > STRING) return RET_FATAL;
			abits = newtypes[abits];
			abits |= ((unsigned char)att & ATTRIBM);
			att = abits;
			form = convertformat(form);
		}
		celladr (cols, &col, &row);
		memset (&cp, 0, sizeof(cellr));
		cpcol(&cp) = col;
		cprow(&cp) = row;
		cpattrib(&cp) = (att & BIMASK) | (att >> 8);
		cpfor(&cp) = form;
		if (cpform(&cp) == DEFAULTFORMAT) cpfor(&cp) |= L_DEFAULT;
		cptype(&cp) = att & TYPEM;
		cpvalue(&cp) = val;
		if (cptype(&cp) == UNITT) {
			cpfor(&cp) |= PROTECT;
			cptext(&cp) = "unit";
			*tex = ' ';
			cpunit(&cp) = tex;
		} else {
			cptext(&cp) = tex+1;
		}
		ct = migratecell(&cp);
		if ((cu = cell(col+1, row)) && cptype(cu)==UNITT) {
				free(cpunit(ct));
				cpunit(ct) = cpunit(cu);
				cpvalue(cu) = cpvalue(ct);
				free(cptext(cu));
				cptext(cu) = cptext(ct);
		}
		if (col > reallastcol) reallastcol = col;
		if (row > reallastrow) reallastrow = row;
		}
	}
lastcol = reallastcol;
lastrow = reallastrow;
if (markfull) {
  omarkcol = omarkrow = 0;
  markcol = lastcol;
  markrow = lastrow;
} else {
  omarkcol = markcol = lastcol + 1;
  omarkrow = markrow = lastrow + 1;
}
return RET_SUCCESS;
} /* loadfile */

int savefile (FILE *file, int scope)
/* Saves the current spreadsheet */
{
char		cols[3];
int		col, row;
int		acol=0, arow=0, ecol=0, erow=0;
CELLPTR		cp;
struct Range	*r;

if (scope == FULL || ftell(file) == 0) {
	fputs (colnames, file);
	fputc ('\n', file);
	fputs (colwidts, file);
	fputc ('\n', file);
	colstring (curcol, cols);
	fprintf (file, "%s%d\t%d\t%d\t%+.*e\twindowdef\n",
		cols, currow+1, windowrow, toprow,
		MAXPLACES, (double)((int)windowline-(int)borderline));
	for (col = 0; col <= lastcol; col++) if (colwidth[col] != defaultwidth)
		{
		colstring (col, cols);
		fprintf (file, "%s\t%d\t%d\t%+.*e\tcoldef\n",
			cols, COLWIDTH, colwidth[col], MAXPLACES, 0.);
		}
	for (r = rnames; r != NULL; r = r->next)
		{
		colstring (r->adr[0].col, cols);
		fprintf (file, "%s%d\t%d\t%d\t%+.*e\trangedef:%s\n",
			cols, r->adr[0].row+1, r->adr[1].col, r->adr[1].row,
			MAXPLACES, 0., r->name);
		}
	}
switch (scope) {
 case FULL:
	arow = 0;
	erow = lastrow;
	acol = 0;
	ecol = lastcol;
	break;
 case RANGES:
	arow = min(omarkrow, markrow);
	erow = max(omarkrow, markrow);
	acol = min(omarkcol, markcol);
	ecol = max(omarkcol, markcol);
	break;
 }
for (row = arow; row <= erow; row++)
 {
 for (col = ecol; col >= acol; col--)
  {
  cp = cell (col, row);
  if (cp != NULL)
	{
	colstring (col, cols);
	fprintf (file, "%s%d\t%d\t%d\t%+.*e\t%s\n",
		cols,
		row+1,
		cpattrbi(cp) | cptype(cp) | cpcolor(cp) << 8,
		cpfor(cp),
		MAXPLACES,
		cpnumber(cp) ? cpvalue(cp) : .0,
		cptype(cp)==UNITT ? cpunit(cp) : cptext(cp)
	 );
	}
  }
 }
return RET_SUCCESS;
} /* savefile */
