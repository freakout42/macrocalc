/* $Id: mcell.c,v 1.12 2001/07/02 10:23:58 axel Exp $
 */

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

static CELLPTR linkcell (int col, int row, CELLPTR cellptr)
/* Links a cell */
{
CELLPTR	*cpp;
CELLPTR	cp;

for (	cpp = &hashcell [hashindex (col, row)];
	(cp = *cpp) != NULL;
	cpp = &cp->next	)

	if (col == cp->adr.col && row == cp->adr.row)
		{
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
	cellptr->adr.col= col;
	cellptr->adr.row= row;
	cellptr->next	= cp ? cp->next : NULL;
	if (col > lastcol) lastcol = col;
	if (row > lastrow) lastrow = row;
	}
return cp;
} /* linkcell */

static void freecell (CELLPTR cp)
/* Free a cell */
{
if (cp==NULL) return;
switch (cptype(cp))
 {
 case STRING:
	free (cpstring(cp));
	/*FALLTHRU*/
 case CONSTANT:
 case VRETRIEVED:
 case FORMULA:
	free (cpv(cp));
 }
if (cptext(cp)) free (cptext(cp));
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

static unsigned char	attrib;
static unsigned char	format;

int deletecell (int col, int row)
/* Deletes a cell */
{
CELLPTR		cp = cell (col, row);

attrib	= 0x00;
format	= 0xff;
if (cp==NULL) return RET_SUCCESS;
attrib	= cpatt(cp);
format	= cpfor(cp);
if (cpprotect(cp)) return RET_ERROR;
if (cpunitf(cp)) {killcell (col+1, row); lcpu(cp) = NULL;}
killcell (col, row);
return RET_SUCCESS;
} /* deletecell */

CELLPTR initcell (int col, int row, unsigned char att, unsigned char form,
		char *s, double val, char *unit)
/* Init a cell */
{
CELLPTR		cp, cput;
int		unitc;

#ifdef DEBUG
fprintf (stderr, "init: c=%3d r=%3d a=%3d f=%3d s=%-16s v=%10le u=\"%-8s\"\n",
	col, row, att, form, s, val, unit);
#endif

assert (col>=0 && col<MAXCOLS && row>=0 && row<MAXROWS);

unitc	= (att==CONSTANT || att==FORMULA || att & UNITF)
		&& unit!=NULL && unit[0]!='\0';
if (deletecell (col, row)) return NULL;
if ((cp = (CELLPTR)(malloc(sizeof(struct CELLREC)))) == NULL) return NULL;
#ifdef DEBUG
fprintf (stderr, "init: cp=%08x\n", cp);
#endif
cpatt(cp) = (att & TYPEM) | (unitc ? UNITF : 0) | ((att | attrib) & ATTRIBM);
cpfor(cp) = format == 0xff ? form : format;
cptext(cp) = strdup (s);
if (cptext(cp)==NULL) return NULL;
#ifdef DEBUG
fprintf (stderr, "init: att=%08x att & TYPEM=%08x\n", att, att & TYPEM);
#endif
switch (att & TYPEM)
 {
 case STRING:
	if (unit==NULL) return NULL;
	/*FALLTHRU*/
 case CONSTANT:
 case VRETRIEVED:
 case FORMULA:
	if ((cpv(cp) = (struct CELLVAL*)(malloc(sizeof(struct CELLVAL))))==NULL)
		return NULL;
	break;
 default:
	cpv(cp)	= NULL;
	break;
 }
#ifdef DEBUG
fprintf (stderr, "init: cpv=%08x\n", cpv(cp));
#endif
switch (att & TYPEM)
 {
 case STRING:
	lcpstring(cp)	= strdup (unit);
	lcplength(cp)	= strlen (unit);
	break;
 case CONSTANT:
 case VRETRIEVED:
 case FORMULA:
	lcpvalue(cp)	= val;
	if (unitc)
		{
		if ((cput = initcell (col+1, row, UNITT,
				(unsigned char)(defaultformat|PROTECT),
				unit, .0, (char*)NULL))
			== NULL) return NULL;
		lcpu(cp) = cptext(cput);
		}
	else	lcpu(cp) = NULL;
	break;
 }
linkcell (col, row, cp);
#ifdef DEBUG
fprintf (stderr, "idon: c=%3d r=%3d a=%3d f=%3d s=%-16s v=%10le u=\"%-8s\"\n",
	col, row, cpatt(cp), cpfor(cp), cptext(cp), cpvalue(cp),
	cpunitf(cp) ? cpunit(cp) : "");
#endif
return cp;
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
