/* mcput.c */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcput.h"
#include "mcolor.h"

void mcrefresh (void)
/* Refresh the entire screen */
{
cur_refr();
} /* mcrefresh */

void writef (int x, int y, int colcode, int width, char *format, ...)
/* Prints a string in video memory at a selected location in a color */
{
va_list	args;
int	oldx, oldy;
char	s[MAXINPUT+1];
int color;
#ifndef NOUTF8
extern int cur_utf8;
char *out = NULL;
wchar_t t[MAXINPUT+1];
#endif

color = mcode2att(colcode & TYPEM);
switch (colcode & BIMASK) {
  case BOLD:   color |= A_BOLD;      break;
  case ITALIC: color |= A_UNDERLINE; break;
}
colcode &= TYPEM;
cur_coor (&oldy, &oldx);
va_start (args, format);
vsnprintf (s, MAXINPUT, format, args);
va_end (args);

cur_satt (stdscr, color);
setcolor(colcode);
s[SCREENWIDTH+1] = '\0';

#ifndef NOUTF8
if (cur_utf8) {
  out = str_sub(out, s, 0, width, 0);
  utf8_to_ucode(t, out, MAXINPUT);
  move (y, x);
  addwstr(t);
  free(out);
} else
#endif
mvprintw (y, x, "%-*s", width, s);

#ifdef DEBUG
fprintf (stderr, "put: y:%2d x:%2d s:\"%-*s\">%08x\n", y, x, width, s, color);
#endif
uncolor(colcode);
cur_satt (stdscr, A_NORMAL);
move (oldy, oldx);
} /* writef */

void clrtoeob (int x1, int y1, int x2)
/* Clears an area of the screen */
{
for (; x1<=x2; x1++)
	{
	move (x1, y1),
	clrtoeol();
	}
}
