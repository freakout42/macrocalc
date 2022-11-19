/* wksfio.c 1.15 2001/11/07 19:18:33 axel */

#pragma GCC diagnostic ignored "-Waddress-of-packed-member"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "lotus.h"
#include "mc2wks.h"
#include "mcelldef.h"
#include "mcommon.h"
#include "mcell.h"
#include "mcellact.h"
#include "mcellstr.h"
#include "mcelladr.h"
#include "mcfileio.h"
#include "mcrange.h"
#include "mconvert.h"
#include "mcparse.h"
#include "wksrecd.h"

int fromwks (FILE *file)
/* Loads a new spreadsheet */
{
int		reallastcol = 0, reallastrow = 0;
int		col, row;
unsigned char	att;
unsigned char	form;
char		tex[MAXINPUT+1];
double		val;
union	{	mydouble c;
		double d;
	}	myd2d;
int		i;
Lotrec		r;
unsigned char	protect	= (unsigned char)~PROTECT;

assert(sizeof(r.data.formula) == LL_formula);
assert(sizeof(r.data.integer) == LL_integer);

/* check if first record is Lotus BOF-record	*/
if (getwksrecord (&r) != L_BOF || r.reclen != LL_bof)
	fatal (10, "no wks-file.");
lib_cano(r.data.bof.ver);
if (	((i = r.data.bof.ver) != T_WKS &&
	  i != T_WKR &&
	  i != T_WK1)	)
	fatal (10, "no wks-file.");

/* read and write all records	*/
while (getwksrecord (&r) != L_EOF)
	{
	switch (r.opcode)
	 {
#ifdef DEBUG
#define printitem(item)	fprintf (stderr, #item "=%6d\n", r.data.?.item)
#endif
	 case L_WINDOW1:
		lib_cano(r.data.window1.curcol);
		lib_cano(r.data.window1.currow);
		lib_cano(r.data.window1.leftcol);
		lib_cano(r.data.window1.windowrow);
		lib_cano(r.data.window1.toprows);
		lib_cano(r.data.window1.toprow);
		curcol		= r.data.window1.curcol;
		currow		= r.data.window1.currow;
		leftcol		= r.data.window1.leftcol;
		windowrow	= r.data.window1.windowrow;
		windowline	= borderline + 1 + r.data.window1.toprows;
		toprow		= r.data.window1.toprow;
		break;
	 case L_COLW1:
		lib_cano(r.data.colw1.col);
		lib_cano(r.data.colw1.width);
		col		= r.data.colw1.col;
		colwidth[col]	= r.data.colw1.width;
		break;
	 case L_NAME:
		lib_cano(r.data.name.cell1.col);
		lib_cano(r.data.name.cell1.row);
		lib_cano(r.data.name.cell2.col);
		lib_cano(r.data.name.cell2.row);
		updaterange (r.data.name.rangename,
			     r.data.name.cell1.col,
			     r.data.name.cell1.row,
			     r.data.name.cell2.col,
			     r.data.name.cell2.row);
		break;
	 case L_PROTEC:
		protect		= ~PROTECT |
					(r.data.protec.protect?PROTECT:0x00);
		break;
	 case L_INTEGER:
		att	= CONSTANT;
		lib_cano(r.data.integer.value);
		val	= r.data.integer.value;
		sprintf (tex, "%d", r.data.integer.value);
		goto allocate;
	 case L_NUMBER:
		att	= CONSTANT;
		myd2d.c	= r.data.number.value;
		val	= lib_iee2 (myd2d.d);
		sprintf (tex, "%f", val);
		goto allocate;
	 case L_LABEL:
	 	convertlabel (&r);
		att	= TEXT;
		val	= 0.;
		strcpy	(tex, r.data.label.s);
		goto allocate;
	 case L_FORMULA:
		att	= FORMULA;
		myd2d.c	= r.data.formula.value;
		val	= lib_iee2 (myd2d.d);
#ifdef DEBUG
		col	=  r.data.label.cell.col;
		row	=  r.data.label.cell.row;
/*		fprintf (stderr, "formula: c=%d r=%d v=%f\n", col, row, val);*/
		fprintf (stderr, "formula: c=%d r=%d\n", col, row);
#endif
		if (pol2tex (tex, r.data.formula.code)) break;
		goto allocate;
	 allocate:
		lib_cano(r.data.label.cell.col);
		lib_cano(r.data.label.cell.row);
		col	=  r.data.label.cell.col;
		row	=  r.data.label.cell.row;
		form	=  convertformat (r.data.label.format) & protect;
		if (!initcell (col, row, att, form, tex, val, NULL))
			return RET_ERROR;
#ifdef DEBUG
		fprintf (stderr, "c=%d r=%d a=%d f=%d t=%s v=%f\n",
			col, row, att, form, tex, val);
#endif
		if (col > reallastcol) reallastcol = col;
		if (row > reallastrow) reallastrow = row;
		break;
	 default:
#ifdef DEBUG
		fprintf (stderr, "opcode=%6d\n", r.opcode);
#endif
		break;
	 }
	}
omarkcol = omarkrow = 0;
markcol = lastcol = reallastcol;
markrow = lastrow = reallastrow;
return RET_SUCCESS;
} /* fromwks */

int towks (FILE *file)
/* Saves the current spreadsheet */
{
int col, row;
struct CELLADR ca;
int type;
char unit[MAXINPUT+1];
CELLPTR cp;
Lotrec r;
struct Range *rg;
union	{	mydouble c;
		double d;
	}	myd2d;

assert(sizeof(r.data.formula) == LL_formula);
assert(sizeof(r.data.integer) == LL_integer);

/* first record is Lotus BOF-record	*/
r.opcode			= L_BOF;
r.reclen			= LL_bof;
r.data.bof.ver			= T_WKS;	/* or T_WK1 ?		*/
lib_cano(r.data.bof.ver);
putwksrecord (&r);
r.opcode			= L_SPLIT;
r.reclen			= LL_split;
r.data.split.splitype		= 0;
putwksrecord (&r);
r.opcode			= L_SYNC;
r.reclen			= LL_split;
r.data.sync.synctype		= 0;
putwksrecord (&r);
r.opcode			= L_WINDOW1;
r.reclen			= LL_window1;
r.data.window1.curcol		= curcol;
r.data.window1.currow		= currow;
r.data.window1.format		= defaultformat;
r.data.window1.unused0		= 0;
r.data.window1.width		= defaultwidth;
r.data.window1.numcols		= rightcol-leftcol+1;
r.data.window1.numrows		= SCREENROWS;
r.data.window1.leftcol		= leftcol;
r.data.window1.windowrow	= windowrow;
r.data.window1.topcols		= 0;
r.data.window1.toprows		= windowline-borderline-1;
r.data.window1.leftcol2		= leftcol;
r.data.window1.toprow		= toprow;
r.data.window1.borderwidthcol	= 4;
r.data.window1.borderwidthrow	= 4;
r.data.window1.windowwidth	= 72;
r.data.window1.unused1		= 0;
lib_cano(r.data.window1.curcol);
lib_cano(r.data.window1.currow);
lib_cano(r.data.window1.format);
lib_cano(r.data.window1.unused0);
lib_cano(r.data.window1.width);
lib_cano(r.data.window1.numcols);
lib_cano(r.data.window1.numrows);
lib_cano(r.data.window1.leftcol);
lib_cano(r.data.window1.windowrow);
lib_cano(r.data.window1.topcols);
lib_cano(r.data.window1.toprows);
lib_cano(r.data.window1.leftcol2);
lib_cano(r.data.window1.toprow);
lib_cano(r.data.window1.borderwidthcol);
lib_cano(r.data.window1.borderwidthrow);
lib_cano(r.data.window1.windowwidth);
lib_cano(r.data.window1.unused1);
putwksrecord (&r);

for (col = 0; col <= lastcol; col++) if (colwidth[col] != 9)
	{
	r.opcode		= L_COLW1;
	r.reclen		= LL_colw1;
	r.data.colw1.col	= col;
	r.data.colw1.width	= colwidth[col];
	lib_cano(r.data.colw1.col);
	lib_cano(r.data.colw1.width);
	putwksrecord (&r);
	}
for (rg = rnames; rg != NULL; rg = rg->next)
	{
	r.opcode		= L_NAME;
	r.reclen		= LL_name;
	strncpy (r.data.name.rangename, rg->name,
		 sizeof(r.data.name.rangename));
	str_upr (r.data.name.rangename);
	r.data.name.cell1.col	= rg->adr[0].col;
	r.data.name.cell1.row	= rg->adr[0].row;
	r.data.name.cell2.col	= rg->adr[1].col;
	r.data.name.cell2.row	= rg->adr[1].row;
	lib_cano(r.data.name.cell1.col);
	lib_cano(r.data.name.cell1.row);
	lib_cano(r.data.name.cell2.col);
	lib_cano(r.data.name.cell2.row);
	putwksrecord (&r);
	}
for (row = 0; row <= lastrow; row++)
 {
 for (col = lastcol; col >= 0; col--)
  {
  cp = cell (col, row);
  if (cp != NULL)
	{
	type	= cptype(cp);
	if (!cpunitf(cp)) unit[0] = '\0';
	origcol = col;
	origrow = row;
#ifdef DEBUG
	fprintf (stderr, "towks: c:%d r:%d = type=%d\n", col, row, type);
#endif
	switch (type)
	 {
	 case STRING:
	 case FORMULA:
		r.opcode		= L_FORMULA;
		myd2d.d			= lib_2iee(cpvalue(cp));
		r.data.formula.value	= myd2d.c;
#ifdef DEBUG
		fprintf (stderr, "towks: &r.data.formula.size=%d\n",
			&r.data.formula.size);
#endif
		type = parse2 (cp, &r.data.formula.size);
		if (type!=FORMULA && type!=STRING && type!=CONSTANT) goto texttype;
		r.reclen = r.data.formula.size + LL_formula - sizeof(r.data.formula.code);
		lib_cano(r.data.formula.size);
		goto recordok;
	 case UNITT:
		unit[0]	= ' ';
		strcpy (unit+1, cptext(cp));
		/*FALLTHRU*/
	 case TEXT:
	 case INCOMMAND:
	 case OUTCOMMAND:
	 case RETRIEVED:
	 texttype:
		r.opcode		= L_LABEL;
		strcpy (r.data.label.s, cptext(cp));
		r.reclen		= r.data.label.s - (char*)&r.data +
					  strlen (r.data.label.s) + 1;
	 	convertlabel (&r);
		goto recordok;
	 case CONSTANT:
	 case VRETRIEVED:
		r.opcode		= L_NUMBER;
		r.reclen		= LL_number;
		myd2d.d			= lib_2iee(cpvalue(cp));
		r.data.number.value	= myd2d.c;
		goto recordok;
	 recordok:
		r.data.label.format	= cp->format;
		ca.col			= col;
		ca.row			= row;
		convertcelladr (&r.data.label.cell, &ca);
		putwksrecord (&r);
		break;
	 case EOFPIPE:
		break;
	 default:
		fprintf (stderr, "mc2wks: unknown type=%d\n", type);
		break;
	 }
	}
  }
 }
/* last record is Lotus BOF-record	*/
r.opcode	= L_EOF;
r.reclen	= LL_eof;
putwksrecord (&r);

return RET_SUCCESS;
} /* towks */
