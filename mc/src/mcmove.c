/* mcmove.c */

#include <stdio.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcdisply.h"
#include "mcwindow.h"
#include "mcfileio.h"
#include "mcmove.h"

#define ACTUALROWS	(SCREENROWS-windowline+borderline+1)

void moverowup (void)
/* Moves up 1 row */
{
displaycur (NOHIGHLIGHT);
if (currow > windowrow) currow--;
else if (windowrow != 0)
	{
	move (windowline, 0);
	insertln ();
	move (curcelline, 0);
	clrtoeol();
	displayrow(--windowrow, NOUPDATE);
	currow--;
	setbottomrow();
	}
} /* moverowup */

void moverowdown (void)
/* Moves down one row */
{
displaycur (NOHIGHLIGHT);
if (currow < bottomrow-1) currow++;
else if (bottomrow < (MAXROWS - 1))
	{
	move (windowline, 0);
	deleteln ();
	move (borderline+SCREENROWS, 0);
	clrtoeol ();
	windowrow++;
	currow++;
	setbottomrow();
	displayrow (bottomrow, NOUPDATE);
	}
} /* moverowdown */

void movecolleft (void)
/* Moves left one column */
{
displaycur (NOHIGHLIGHT);
if (curcol > leftcol) curcol--;
else if (leftcol != 0)
	{
	curcol--;
	leftcol--;
	setrightcol();
	clearlastcol();
	displayscreen(NOUPDATE);
	}
} /* movecolleft */

void movecolright (void)
/* Moves right one column */
{
displaycur (NOHIGHLIGHT);
if (curcol < rightcol) curcol++;
else if (rightcol < (MAXCOLS - 1))
	{
	curcol++;
	rightcol++;
	setleftcol();
	clearlastcol();
	displayscreen(NOUPDATE);
	}
} /* movecolright */

void movepageup (void)
{
displaycur (NOHIGHLIGHT);
windowrow -= ACTUALROWS;
currow -= ACTUALROWS;
if (currow < 0)	currow = windowrow = 0;
else if (windowrow < 0) windowrow = 0;
setbottomrow();
displayscreen(NOUPDATE);
} /* movepageup */

void movepagedown (void)
{
displaycur (NOHIGHLIGHT);
windowrow += ACTUALROWS;
currow += ACTUALROWS;
if ((currow >= MAXROWS) && (windowrow >= MAXROWS))
	{
	currow = MAXROWS - 1;
	windowrow = MAXROWS - ACTUALROWS;
	}
else if (windowrow > (MAXROWS - ACTUALROWS))
	{
	currow -= (windowrow + ACTUALROWS - MAXROWS);
	windowrow = MAXROWS - ACTUALROWS;
	}
setbottomrow();
displayscreen(NOUPDATE);
} /* movepagedown */

void movepageleft (void)
{
displaycur (NOHIGHLIGHT);
if (leftcol == 0) curcol = 0;
else
	{
	curcol = rightcol = leftcol - 1;
	setleftcol();
	displayscreen(NOUPDATE);
	}
} /* movepageleft */

void movepageright (void)
{
displaycur (NOHIGHLIGHT);
if (rightcol == MAXCOLS - 1) curcol = rightcol;
else
	{
	curcol = leftcol = rightcol + 1;
	setrightcol();
	displayscreen(NOUPDATE);
	}
} /* movepageright */

void movehome (void)
{
displaycur (NOHIGHLIGHT);
curcol = leftcol = 0;
currow = windowrow = topbotrow + 1;
setrightcol();
setbottomrow();
displayscreen(NOUPDATE);
}

void moveend (void)
{
displaycur (NOHIGHLIGHT);
curcol = rightcol = lastcol;
currow = bottomrow = lastrow;
setleftcol();
settoprow();
displayscreen(NOUPDATE);
}
