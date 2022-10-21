/* $Id: mcdisply.c,v 1.14 2001/07/02 10:23:58 axel Exp $
 */

#ifdef DEBUG
#include <stdio.h>
#endif
#include <string.h>
#include <arx_def.h>
#include <cur_def.h>
#include <term.h>
#include "mc.h"
#include "mcput.h"
#include "mcutil.h"
#include "mcell.h"
#include "mcellstr.h"
#include "mcelladr.h"
#include "mcwindow.h"
#include "mcrecalc.h"
#include "mcfileio.h"
#include "mcdisply.h"

void printcol (void)
/* Prints the column headings */
{
int col;
char colstr[MAXSCREENWIDTH+1];

if (!borders) return;
writef(0, borderline, HEADERCOLOR, LEFTMARGIN, "");
for (col = leftcol; col <= rightcol; col++)
	{
	centercolstring(col, colstr);
	writef(colstart[col - leftcol], borderline,
		HEADERCOLOR, colwidth[col], colstr);
#ifdef DEBUG
fprintf (stderr, "printcol: %d\n", col);
#endif
	}
} /* printcol */

void clearlastcol (void)
/* Clears any data left in the last column */
{
int col;

if ((col = colstart[rightcol - leftcol] + colwidth[rightcol]) < SCREENWIDTH)
	clrtoeob (borderline, col, borderline+SCREENROWS);
} /* clearlastcol */

void printrow (void)
/* Prints the row headings */
{
unsigned int row, line;

if (!borders) return;
for (row = toprow, line = borderline+1; line < windowline; row++, line++)
	writef(0, line, HEADERCOLOR, LEFTMARGIN, "%-d", row + 1);
for (row = windowrow, line = windowline; row <= bottomrow; row++, line++)
	writef(0, line, HEADERCOLOR, LEFTMARGIN, "%-d", row + 1);
} /* printrow */

void displaycell (int col, int row, int highlighting, int updating)
/* Displays the contents of a cell */
{
int	color;
char	*s;
CELLPTR	cp = cell (col, row);

if ((updating && cp != NULL && cptype(cp) != FORMULA) || col < leftcol || col > rightcol) return;
s = cellstring (col, row, &color, FORMAT);
if ((highlighting) ^ (
			highlight &&
			col >= omarkcol &&
			row >= omarkrow &&
			col <= markcol  &&
			row <= markrow  &&
			(!highlighting ||
			 omarkcol!=markcol || omarkrow!=markrow)
								))
	{
	color = color&ERRORCOLOR ? HIGHLIGHTERRORCOLOR : HIGHLIGHTCOLOR;
	}
#ifdef DEBUG
fprintf (stderr, "displaycell: [%d,%d]=%s color=%d\n", col, row, s, color);
#endif
if (row >= toprow && row <= topbotrow)
	writef (colstart[col - leftcol], row-toprow+borderline+1,
		color, colwidth[col], "%s", s);
if (row >= windowrow && row <= bottomrow)
	writef (colstart[col - leftcol], row-windowrow+windowline,
		color, colwidth[col], "%s", s);
} /* displaycell */

void displaycol (int col, int updating)
/* Displays a column on the screen */
{
int row;

for (row = toprow; row <= topbotrow; row++)
 	displaycell(col, row, NOHIGHLIGHT, updating);
for (row = windowrow; row <= bottomrow; row++)
 	displaycell(col, row, NOHIGHLIGHT, updating);
} /* displaycol */

void displayrow (int row, int updating)
/* Displays a row on the screen */
{
int col;

for (col = leftcol; col <= rightcol; col++)
	displaycell(col, row, NOHIGHLIGHT, updating);
} /* displayrow */

void displaycur (int highlighting)
/* Displays the current cell */
{
int	color;
char	colstr[MAXSCREENWIDTH+1];

if (highlighting) displayrow (currow, NOUPDATE);
color	= highlighting ? CURHEADERCOLOR : HEADERCOLOR;
centercolstring (curcol, colstr);
displaycell (curcol, currow, highlighting, NOUPDATE);

if (!borders) return;
writef (colstart[curcol - leftcol], borderline,
	color, colwidth[curcol], colstr);
writef(0, currow-windowrow+windowline,
	color, LEFTMARGIN, "%-d", currow + 1);
} /* displaycur */

void displayscreen (int updating)
/* Displays the current screen of the spreadsheet */
{
int row;

printcol();
printrow();
for (row = toprow; row <= topbotrow; row++) displayrow(row, updating);
for (row = windowrow; row <= bottomrow; row++) displayrow(row, updating);
clearlastcol();
} /* displayscreen */

void showcelltype (void)
/* Prints the type of cell and what is in it */
{
char	c[MAXADR+1], f, a;
char	*t;
int	p;
int	color;

if (!cellcont) return;
formdisplay = !formdisplay;
celladrstring (curcol, currow, c);
if (curcell == NULL)
	{
	t = TXTEMPTY;
	f = ' ';
	a = ' ';
	p = 0;
	}
else
	{
	switch ( cptype(curcell)	)
	 {
	 case TEXT:
		t	= TXTTEXT;
		break;
	 case CONSTANT:
		t	= TXTVALUE;
		break;
	 case FORMULA:
		t	= TXTFORMULA;
		break;
	 case STRING:
		t	= TXTSTRING;
		break;
	 case UNITT:
		t	= TXTUNIT;
		break;
	 case INCOMMAND:
	 case OUTCOMMAND:
		t	= TXTCMD;
		break;
	 case RETRIEVED:
	 case VRETRIEVED:
		t	= TXTRETR;
		break;
	 case EOFPIPE:
		t	= TXTEOF;
		break;
	 default:
		t	= TXTUNKNOWN;
		break;
	 } /* switch */
	switch ( cpformat(curcell) )
	 {
	 case FIXED:
		f	= 'F';
		break;
	 case SCIENTIFIC:
		f	= 'S';
		break;
	 case CURRENCY:
		f	= 'C';
		break;
	 case PERCENT:
		f	= '%';
		break;
	 case COMMA:
		f	= ',';
		break;
	 case SPECIAL:
		f	= 'x';
		break;
	 } /* switch */
	switch ( cpattrib(curcell) )
	 {
	 case BOLD:
		a	= 'B';
		break;
	 case ITALIC:
		a	= 'I';
		break;
	 default:
		a	= ' ';
		break;
	 } /* switch */
	p	= cpplaces (curcell);
	}
writef	(0, curcelline, CELLCONTENTSCOLOR, SCREENWIDTH, "%s %s [%c%d%c]: %s",
	c, t, f, p, a, cellstring (curcol, currow, &color, NOFORMAT));
formdisplay = !formdisplay;
} /* showcelltype */

void redrawscreen (void)
/* Displays the entire screen */
{
if (columns!=0) screencol = columns; /* set screen dimensions */
if (lines!=0) screenlin = lines;
if (borderline==0) { /* -l layout */
	curcelline	= SCREENROWS+1;
	errorline	= SCREENROWS+2;
	inputline	= SCREENROWS+3;
	cmdline		= SCREENROWS+3;
	windowline	= 1;
	}
setrightcol();
setbottomrow();
displayscreen(NOUPDATE);
} /* redrawscreen */

void displaymark (void)
/* Print marked positions on the screen */
{
char	*p, s[MAXLEFTMARGIN*2+6];

p = celladrstring (omarkcol, omarkrow, s);
strcat (p, ":");
p = celladrstring (markcol, markrow, p+1);
writef (MARKPOS, errorline, MARKCOLOR, strlen(p), s);
} /* displaymark */

void changeautocalc (int newmode)
/* Changes and prints the current AutoCalc value on the screen */
{
char s[15]	= TXTAUTOCALC;

if (!autocalc && newmode) recalc();
autocalc = (char)newmode;
writef (AUTOPOS, errorline, AUTOCALCCOLOR, strlen(s), autocalc ? s : "");
} /* changeautocalc */

void changeformdisplay (int newmode)
/* Changes and prints the current formula display value on the screen */
{
char s[15]	 = TXTFORMDISPLAY;

writef (FORMPOS, errorline, FORMDISPLAYCOLOR, strlen(s), newmode ? s : "");
if (formdisplay != newmode)
	{
	formdisplay = (char)newmode;
	displayscreen (UPDATE);
	}
} /* changeformdisplay */

void mcsystem (char *command, int prompt)
{
cur_syst (stdscr, command, prompt);
mcrefresh();
}
