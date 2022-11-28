/* mcpipe.c 1.11 1997/04/05 15:47:34 axel */

#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcellact.h"
#include "mcellpar.h"
#include "mcelladr.h"
#include "mcellstr.h"
#include "mcparse.h"
#include "mcfileio.h"
#include "mcpipe.h"

int inpipe (int col, int row, char *cmd)
/* Get from pipe input */
{
FILE	*p;
char	buf[MAXINPUT+2];
char	tab	= '\t';
char	*t, *next;
int	acol;
CELLPTR	cp;
cellr	cr;
double	val;
char	att;
int	insert	= FALSE;
int	formula	= FALSE;

#ifdef DEBUG
fprintf(stderr, "inpipe: col=%d row=%d cmd=%s\n", col, row, cmd);
#endif
switch(*cmd) {
 case '<':
	insert	= TRUE;
	cmd++;
	break;
 case '!':
	formula	= TRUE;
	cmd++;
	break;
 }
if (!(p = popen(cmd, "r"))) return RET_ERROR;
while (str_gets(p, buf+1, MAXINPUT)) {
	if (insert) {
		moverange(0, row+1, 0, row, MAXCOLS, MAXROWS);
		copyrange(0, row, 0, row+1, lastcol, row+1);
	}
	acol = col-1;
	next = buf+1;
	while (next != NULL) {
		acol++;
	  cp = cell (acol, row);
		if (acol>=MAXCOLS) break;
		t = next;
		if ((next = strchr (t, tab)) != NULL) *next++ = '\0';
		if (*t == '\0')	{
			deletecell(acol, row);
			continue;
		}
		if (formula) {
			cp = parsecell(t, acol, row);
		} else {
			memset (&cr, 0, sizeof(cellr));
			cpcol(&cr) = acol;
			cprow(&cr) = row;
			if ((val = str_chkd(t)) == HUGE_VAL) {
				cptype(&cr) = RETRIEVED;
				*--t = STRLEFT;
			} else {
				cptype(&cr) = VRETRIEVED;
				cpvalue(&cr) = val;
			}
			cptext(&cr) = t;
			cp = migratcell(cp, &cr);
		}
	}
	row++;
	if (row>=MAXROWS-1) break;
	}
pclose(p);
cp = initcell(col, row, EOFPIPE, defaultformat, "EOF", 0., NULL);
return cp==NULL ? EOF : RET_SUCCESS;
} /* inpipe */

int outpipe (int col, int row, char *cmd)
/* Put into output pipe */
{
FILE	*p;
char	tab = '\t';
char	*s;
int	acol, ncol;
CELLPTR	cp;
int	color;

for (ncol = col; (cp = cell(ncol, row)); ncol++);
if (ncol == col) return RET_SUCCESS;

if (!(p = popen(cmd, "w"))) return RET_ERROR;
acol = col;
while (row <= lastrow)
	{
	cp = cell(acol, row);
	if (cp != NULL)
		{
		if (cptype(cp) == EOFPIPE) break;
		s = cellstring(acol, row, &color, NOCOLUMN);
		fputs(s, p);
		}
	acol++;
	if (acol >= ncol)
		{
		acol = col;
		row++;
		fputc('\n', p);
		}
	else	fputc(tab, p);
	}
return pclose(p);
} /* outpipe */

void inpipeall (void)
{
int	col, row;
int	dcol, drow;
CELLPTR	cp;
int	type;
char	parsed[MAXPARSED+1];
int	clen;

for (row = 0; row <= lastrow; row++)
 {
 for (col = lastcol; col >= 0; col--)
  {
  cp = cell(col, row);
  if (cp != NULL)
	{
	origcol	= col;
	origrow	= row;
	parse(cp, cptext(cp), parsed);
	if (type == INCOMMAND)
		{
		clen = celladr(parsed, &dcol, &drow);
		inpipe(dcol, drow, parsed+clen+1);
		}
	}
  }
 }
}

void outpipeall (void)
{
int	col, row;
int	dcol, drow;
CELLPTR	cp;
int	type;
char	parsed[MAXPARSED+1];
int	clen;

for (row = 0; row <= lastrow; row++)
 {
 for (col = lastcol; col >= 0; col--)
  {
  cp = cell(col, row);
  if (cp != NULL)
	{
	origcol	= col;
	origrow	= row;
	parse(cp, cptext(cp), parsed);
	if (type == OUTCOMMAND)
		{
		clen = celladr(parsed, &dcol, &drow);
		outpipe(dcol, drow, parsed+clen+1);
		}
	}
  }
 }
}
