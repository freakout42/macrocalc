/* mcwindow.c,v 1.8 2001/06/06 06:09:29 axel */

#ifdef DEBUG
#include <stdio.h>
#endif
#include <string.h>
#include "mc.h"
#include "mcdisply.h"
#include "mcmessag.h"
#include "mcwindow.h"

#define ACTUALROWS	(SCREENROWS-windowline+borderline+(borders?2:1))

void setrightcol (void)
/* Sets the value of rightcol based on the value of leftcol */
{
unsigned int total = LEFTMARGIN, col = 0;
do
	{
	colstart[col] = total;
	total += colwidth[leftcol + col];
	} while ((total <= SCREENWIDTH) && (leftcol + ++col < MAXCOLS));
rightcol = leftcol + col - 1;
#ifdef DEBUG
fprintf (stderr, "setrightcol: %d->%d\n", leftcol, rightcol);
#endif
} /* setrightcol */

void setleftcol (void)
/* Sets the value of leftcol based on the value of rightcol */
{
int total = SCREENWIDTH, col = 0;

/* last col++ suppressed if width overflow */
while ((total >= LEFTMARGIN) && (rightcol - col++ >= 0))
	{
	total -= colwidth[rightcol - col + 1];
#ifdef DEBUG
	fprintf (stderr, "setleftcol: col=%d width=%d total=%d\n",
		rightcol-col+1, colwidth[rightcol-col+1], total);
#endif
	}
leftcol = rightcol - col + 2;
#ifdef DEBUG
fprintf (stderr, "setleftcol1: %d->%d\n", rightcol, leftcol);
#endif
setrightcol();
#ifdef DEBUG
fprintf (stderr, "setleftcol2: %d-%d\n", rightcol, leftcol);
#endif
} /* setleftcol */

static void setleftmargin(void) {
int oldleft = leftmargin;
if (bottomrow>=10000) leftmargin=5;
else if (bottomrow>=1000) leftmargin=4;
else leftmargin=3;
if (!borders) leftmargin=0;
if (leftmargin != oldleft) setrightcol();
} /* setleftmargin */

void settoprow (void)
/* Figures out the value of windowrow based on the value of bottomrow */
{
if (bottomrow - ACTUALROWS < -1) bottomrow = ACTUALROWS-1;
windowrow = bottomrow - ACTUALROWS + 1;
setleftmargin();
} /* settoprow */

void setbottomrow (void)
/* Figures out the value of bottomrow based on the value of windowrow */
{
if (windowrow + ACTUALROWS > MAXROWS) windowrow = MAXROWS - ACTUALROWS;
topbotrow = toprow + (windowline - borderline - 2);
bottomrow = windowrow + ACTUALROWS - 1;
setleftmargin();
} /* setbottomrow */

void horizwindow (void)
{
if (windowline != borderline+1) {errormsg(MSGWINDOW); return;}
toprow		= windowrow;
windowrow	= currow;
windowline	= borderline+1+currow-toprow;
setbottomrow();
displayscreen(NOUPDATE);
}

void clearwindow (void)
{
toprow		= 0;
windowline	= borderline+1;
setbottomrow();
displayscreen(NOUPDATE);
}
