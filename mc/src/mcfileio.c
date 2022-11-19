/* mcfileio.c 1.10 2004/07/04 04:03:31 axel */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcellact.h"
#include "mcellstr.h"
#include "mcelladr.h"
#include "mcfilef.h"
#include "mcrange.h"
#include "mcfileio.h"

int lastcol, lastrow, markcol=0, markrow=0, omarkcol=0, omarkrow=0;

int loadfile (FILE *file)
/* Loads a new spreadsheet */
{
int	reallastcol = lastcol, reallastrow = lastrow;
char	cols[8];
char	line[MAXINPUT+1];
int	col, row;
int	att;
int	form;
char	tex[MAXINPUT+1];
double	val;
char	unit[MAXINPUT+1];
int	loaded = 0;

str_gets (file, line, sizeof(line));
if (strcmp(line, colnames) != 0) return RET_FATAL;
str_gets (file, line, sizeof(line));
if (strspn(line, "-\t") != strlen(line)) return RET_FATAL;
while (fscanf (file, "%s\t%d\t%d\t%lf\t", cols, &att, &form, &val)==4)
	{
	str_gets (file, tex, MAXINPUT);
	celladr (cols, &col, &row);
	if (!strcmp (tex, "windowdef"))
		{
		curcol		= col;
		currow		= row;
		windowrow	= att;
		toprow		= form;
		windowline	= borderline + (int)val;
		}
	else if (!strcmp (tex, "coldef"))
		{
		celladr (cols, &col, &row);
		colwidth[col]	= (unsigned char) form;
		}
	else if (!strncmp (tex, "rangedef:", 9))
		{
		updaterange (tex+9, col, row, att, form);
		}
	else
		{
		celladr (cols, &col, &row);
		if ((att & TYPEM) == UNITT)
			{
			unit[0]	= ' ';
			strcpy (unit+1, tex);
			}
		else	
			{
			unit[0] = '\0';
			if (!initcell(col, row, att, form, tex, val, unit))
				return RET_ERROR;
			}
		if (col > reallastcol) reallastcol = col;
		if (row > reallastrow) reallastrow = row;
		}
	if (loaded++>40 && license!=0) break;
	}
omarkcol = omarkrow = 0;
markcol = lastcol = reallastcol;
markrow = lastrow = reallastrow;
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
		cpatt(cp),
		cpfor(cp),
		MAXPLACES,
		cpnumber(cp) ? cpvalue(cp) : .0,
		cptext(cp));
	}
  }
 }
return RET_SUCCESS;
} /* savefile */
