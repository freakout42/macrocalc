/* mcell.c 1.12 2001/07/02 10:23:58 axel */

#ifdef DEBUG
#include <stdio.h>
#endif

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcunit.h"
#include "mcfileio.h"
#include "mcell.h"
#include "mcelladr.h"

#define	COLBITS		6
#define COLHASH		(1<<COLBITS)
#define	ROWBITS		10
#define ROWHASH		(1<<ROWBITS)
#define HASHCELLS	(COLHASH*ROWHASH)

static CELLPTR hashcell [HASHCELLS];
#define hashindex(col,row) (((row & (ROWHASH-1)) << COLBITS) | (col & (COLHASH-1)))

void inithash (void)
{
memset (hashcell, 0, sizeof(hashcell));
}

void sheetwalk (CELLPTR (*fcn)(int col, int row))
/* walks through all cells of sheet fast */
{
register int i;
CELLPTR	cp;

for (i = 0; i < HASHCELLS; i++)
  if (hashcell[i] != NULL)
    for (cp = hashcell[i]; cp != NULL; cp = cp->next)
	(*fcn) (cp->adr.col, cp->adr.row);
return;
}

CELLPTR cell (int col, int row)
/* Get a cell */
{
CELLPTR	cp;

if (col<0 || col>=MAXCOLS || row<0 || row>=MAXROWS)
	return NULL;
for (cp = hashcell [hashindex (col, row)]; cp; cp = cp->next)
	if (col == cp->adr.col && row == cp->adr.row) return cp;
return NULL;
}

static void setlastcol (void)
/* Sets the value of lastcol based on the current value */
{
register int row, col;

for (col = lastcol; col >= 0; col--)
	{
	for (row = 0; row <= lastrow; row++)
		{
		if (cell (col, row) != NULL)
			{
			lastcol = col;
			return;
			}
		}
	}
lastcol = 0;
} /* setlastcol */

static void setlastrow (void)
/* Sets the value of lastrow based on the current value */
{
register int row, col;

for (row = lastrow; row >= 0; row--)
	{
	for (col = 0; col <= lastcol; col++)
		{
		if (cell (col, row) != NULL)
			{
			lastrow = row;
			return;
			}
		}
	}
lastrow = 0;
} /* setlastrow */

static CELLPTR linkcell (int col, int row, CELLPTR cellptr) {
/* Links a cell */
CELLPTR	*cpp;
CELLPTR	cp;

for (	cpp = &hashcell [hashindex (col, row)];
	(cp = *cpp) != NULL;
	cpp = &cp->next	)

	if (col == cpcol(cp) && row == cprow(cp)) {
		if (cellptr == NULL)
			{
			*cpp		= cp->next;
			if (col >= lastcol) setlastcol();
			if (row >= lastrow) setlastrow();
			return cp;
			}
		else
			break;
		}

if (cellptr != NULL)
	{
	*cpp		= cellptr;
	cpcol(cellptr) = col;
	cprow(cellptr) = row;
	cpnext(cellptr) = cp ? cpnext(cp) : NULL;
	if (col > lastcol) lastcol = col;
	if (row > lastrow) lastrow = row;
	}
return cp;
} /* linkcell */

void freecell (CELLPTR cp)
/* Free a cell */
{
if (cp==NULL) return;
if (cptext(cp)) free(cptext(cp));
if (cpunit(cp)) free(cpunit(cp));
if (cpstring(cp)) free(cpstring(cp));
free (cp);
}

static void killcell (int col, int row)
/* Kill a cell */
{
#ifdef DEBUG
fprintf (stderr, "kill: c=%d r=%d\n", col, row);
#endif
freecell (linkcell (col, row, (CELLPTR)NULL));
} /* killcell */

int deletecell (int col, int row)
/* Deletes a cell */
{
CELLPTR		cp = cell (col, row);

if (cp==NULL) return RET_SUCCESS;
if (cpprotect(cp)) return RET_ERROR;
if (cpsidecar(cp)) {killcell (col+1, row);}
killcell (col, row);
return RET_SUCCESS;
} /* deletecell */

static void strdupi(char **t, char *s) {
if (s == NULL) {
 free(*t);
 *t = NULL;
} else if (*t == NULL) {
 *t = strdup(s);
} else {
 if (strcmp(*t, s)) {
  if (strlen(*t) < strlen(s)) {
   realloc(*t, strlen(s)+1);
  }
 strcpy(*t, s);
 }
}
return;
}
CELLPTR migratcell(CELLPTR ct, CELLPTR cs) {
/* Migrate a cell */
if (ct == NULL) {
  ct = newcell();
  linkcell (cpcol(cs), cprow(cs), ct);
  cpattrib(ct) = cpattrib(cs);
  cpfor(ct) = cpfor(cs);
}
cptype(ct) = cptype(cs);
strdupi(&cptext(ct), cptext(cs));
cpvalue(ct) = cpvalue(cs);
cpcimag(ct) = cpcimag(cs);
strdupi(&cpunit(ct), cpunit(cs));
cplength(ct) = cplength(cs);
strdupi(&cpstring(ct), cpstring(cs));
/*
		if ((cput = initcell (col+1, row, UNITT,
				(unsigned char)(defaultformat|PROTECT),
				unit, .0, (char*)NULL))
			== NULL) return NULL;
		cpunit(cp) = cptext(cput);
 */
return ct;
}

CELLPTR init2cell(int col, int row, int type, char *text, double value, double cimag, char *unit) {
CELLPTR	cp;
cellr	cr;

cp = cell(col, row);
memset(&cr, 0, sizeof(cellr));
cpcol(&cr) = col;
cprow(&cr) = row;
cptype(&cr) = type;
cpvalue(&cr) = value;
cpcimag(&cr) = cimag;
cptext(&cr) = text;
cpunit(&cr) = unit;
return migratcell(cp, &cr);
}

int movecell (int tcol, int trow, int scol, int srow)
/* Moves a cell */
{
CELLPTR	cp;

if ((cp = linkcell (tcol, trow, linkcell (scol, srow, (CELLPTR)NULL))))
	freecell (cp);
return RET_SUCCESS;
} /* movecell */

char *celltext (int col, int row)
/* Finds text of a particular cell */
{
CELLPTR cp = cell (col, row);

if (cp == NULL) return NULL;
switch (cptype(cp))
 {
 case TEXT:
 case RETRIEVED:	return cptext(cp)+1;
 case STRING:		return cpstring(cp);
 default:		return cptext(cp);
 }
} /* celltext */
