/* mcellact.c 1.13 2004/12/11 16:21:11 axel */

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
#include "mccalc.h"
#include "mcrecalc.h"
#include "mcmessag.h"
#include "mcdisply.h"	/* not nice! */
#include "mcwindow.h"	/* not nice! */
#include "mcparse.h"
#include "mcfileio.h"
#include "mcellact.h"

static int cborder (int *target, int *start, int *send)
{
if (*start > *send) swap (start, send);
if (*start > *target)
	{
	return 1;
	}
else
	{
	*target += *send - *start;
	swap (start, send);
	return -1;
	}
}

int copycell (int tcol, int trow, int scol, int srow)
/* Copies a cell */
{
CELLPTR	scp, tcp;

if (tcol<0 || tcol>=MAXCOLS || trow<0 || tcol>=MAXROWS) return RET_SUCCESS;
if ((scp = cell (scol, srow)) != NULL)
	{
	tcp = initcell (tcol, trow, cpattrib(scp), cpformat(scp), cptext(scp), 0.,
		cptype(scp)==STRING ? cpstring(scp) : cpunit(scp));
	if (tcp == NULL) return RET_ERROR;
	if (cptype(scp)==CONSTANT || cptype(scp)==VRETRIEVED)
		cpvalue(tcp) = cpvalue(scp);
	else
		recalcell (tcol, trow);
	}
else
	{
	deletecell (tcol, trow);
	}
return RET_SUCCESS;
} /* copycell */

int inrange (int col, int row, int scol, int srow, int ecol, int erow)
{
return
 	adrval(col) >= min (scol, ecol) &&
	adrval(col) <= max (scol, ecol) &&
	adrval(row) >= min (srow, erow) &&
	adrval(row) <= max (srow, erow);
}

static void adjustcell (int *col, int *row,
	int tcol, int trow, int scol, int srow, int ecol, int erow)
{
#ifdef DEBUG
fprintf (stderr, "adjustcell: col=%d row=%d tcol=%d trow=%d scol=%d srow=%d ecol=%d erow=%d ",
	*col, *row, tcol, trow, scol, srow, ecol, erow);
#endif
if (inrange (*col, *row, scol, srow, ecol, erow))
	{
	if (*col>=0) *col += tcol-scol;
	else	     *col -= tcol-scol;
	if (*row>=0) *row += trow-srow;
	else	     *row -= trow-srow;
	}
#ifdef DEBUG
fprintf (stderr, "-> col=%d row=%d\n", *col, *row);
#endif
}

void moverange (int tcol, int trow, int scol, int srow, int ecol, int erow)
/* Moves a cellrange */
{
int		cinc, rinc;
int		ccnt, rcnt;
int		tccnt, trcnt;
char		*s, newformula[MAXINPUT + 1], *t;
int		fcol, frow, n;
CELLPTR		cp;
struct Range	*r;

for (r = rnames; r != NULL; r = r->next)
	{
	adjustcell (&r->adr[0].col, &r->adr[0].row,
		tcol, trow, scol, srow, ecol, erow);
	adjustcell (&r->adr[1].col, &r->adr[1].row,
		tcol, trow, scol, srow, ecol, erow);
	}
for (ccnt = 0; ccnt <= lastcol; ccnt++)
    for (rcnt = 0; rcnt <= lastrow; rcnt++)
	{
	if ((cp = cell (ccnt, rcnt)) != NULL && cpformula(cp))
		{
		t = newformula;
		for (s = cptext(cp); *s;)
			{
			origcol = ccnt;
			origrow = rcnt;
			if ((n = cellaadr (s, &fcol, &frow)) != 0)
				{
				adjustcell (&fcol, &frow,
					tcol, trow, scol, srow, ecol, erow);
				adjustcell (&origcol, &origrow,
					tcol, trow, scol, srow, ecol, erow);
				t = celladrstring (fcol, frow, t);
				s += n;
				}
			else	*t++ = *s++;
			}
		*t = '\0';
		cptext(cp) = (char *)realloc (cptext(cp), strlen(newformula)+1);
		strcpy (cptext(cp), newformula);
		}
	}
if (ecol == MAXCOLS) ecol = lastcol;
if (erow == MAXROWS) erow = lastrow;
cinc = cborder (&tcol, &scol, &ecol);
rinc = cborder (&trow, &srow, &erow);
for (	ccnt = scol, tccnt = tcol;
	(ecol-ccnt)*cinc >= 0;
	ccnt += cinc, tccnt += cinc	)
    {
    for (	rcnt = srow, trcnt = trow;
		(erow-rcnt)*rinc >= 0;
		rcnt += rinc, trcnt += rinc	)
	{
	movecell (tccnt, trcnt, ccnt, rcnt);
	}
    }
changed = TRUE;
} /* moverange */

void copyrange (int tcol, int trow, int scol, int srow, int ecol, int erow)
/* Copies a cellrange */
{
int	cinc, rinc;
int	ccnt, rcnt;
int	tccnt, trcnt;

cinc	= cborder (&tcol, &scol, &ecol);
rinc	= cborder (&trow, &srow, &erow);
for (	ccnt = scol, tccnt = tcol;
	(ecol-ccnt)*cinc >= 0;
	ccnt += cinc, tccnt += cinc	)
    {
    for (	rcnt = srow, trcnt = trow;
		(erow-rcnt)*rinc >= 0;
		rcnt += rinc, trcnt += rinc	)
	{
	if (copycell (tccnt, trcnt, ccnt, rcnt))
		{
		errormsg (MSGLOMEM);
		}
	}
    }
changed = TRUE;
} /* copyrange */

void deletecol (int col)
/* Deletes a column */
{
int	row;
int	ccnt;

for (ccnt=col; ccnt<MAXCOLS; ccnt++) colwidth[ccnt] = colwidth[ccnt+1];
setrightcol();
if (col > lastcol)	return;
else if (col < lastcol)
	moverange (col, 0, col+1, 0, MAXCOLS, MAXROWS);
else
	for (row=0; row<=lastrow; row++) deletecell (col, row);
while (col <= rightcol) displaycol(col++, NOUPDATE);
changed = TRUE;
autorecalc();
} /* deletecol */

void insertcol(col) int col;
/* Inserts a column */
{
int	ccnt;

for (ccnt=MAXCOLS; ccnt>col; ccnt--) colwidth[ccnt] = colwidth[ccnt-1];
colwidth[col] = defaultwidth;
setrightcol();
moverange (col+1, 0, col, 0, MAXCOLS, MAXROWS);
while (col <= rightcol) displaycol(col++, NOUPDATE);
changed = TRUE;
autorecalc();
} /* insertcol */

void deleterow (int row)
/* Deletes a row */
{
int	col;

if (row > lastrow)	return;
else if (row < lastrow)
	moverange (0, row, 0, row+1, MAXCOLS, MAXROWS);
else
	for (col=0; col<=lastcol; col++) deletecell (col, row);
while (row <= bottomrow) displayrow(row++, NOUPDATE);
changed = TRUE;
autorecalc();
} /* deleterow */

void insertrow (row) int row;
/* Inserts a row */
{
moverange (0, row+1, 0, row, MAXCOLS, MAXROWS);
while (row <= bottomrow) displayrow(row++, NOUPDATE);
changed = TRUE;
autorecalc();
} /* insertrow */

double cellvalue (int col, int row)
/* Finds the value of a particular cell */
{
CELLPTR cp = cell (col, row);

return (cp!=NULL && cpformula(cp)) ? cpvalue(cp) : .0;
} /* cellvalue */
