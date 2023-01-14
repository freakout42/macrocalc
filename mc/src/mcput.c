/* mcput.c,v 1.5 1995/05/20 16:21:37 axel */

#include <stdio.h>
#include <stdarg.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcput.h"
#include "mcolor.h"

void mcrefresh (void)
/* Refresh the entire screen */
{
clearok (stdscr, TRUE);
return;
} /* mcrefresh */

void writef (int x, int y, int colcode, int width, char *format, ...)
/* Prints a string in video memory at a selected location in a color */
{
va_list	args;
int	oldx, oldy;
char	s[MAXINPUT+1];
int color;

color = mcode2att(colcode);
getyx (stdscr, oldy, oldx);
va_start (args, format);
vsprintf (s, format, args);
va_end (args);

#ifdef CURSESBUG
mvaddch (0, 79, ' ');
refresh();
#endif

cur_satt (stdscr, color);
s[SCREENWIDTH+1] = '\0';
mvprintw (y, x, "%-*s", width, s);
#ifdef DEBUG
fprintf (stderr, "put: y:%2d x:%2d s:\"%-*s\">%08x\n", y, x, width, s, color);
#endif
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

#ifdef CURSESBUG
	mvaddch (x1, 79, ' ');
#endif

	}
}
