/* cur_goll.c
 * curses misc output
 */

#include <stdio.h>
#include <cur_def.h>
#include <term.h>

void cur_coor (int *y, int *x) {
int	cy=0, cx=0; /* screen positions */
getyx (stdscr, cy, cx);
*y = cy;
*x = cx;
}

void cur_refr (void)
/* Refresh the entire screen */
{
clearok (stdscr, TRUE);
return;
}

int cur_tput (int c)
{
putchar (c);
fflush (stdout);
return c;
}

void cur_goll (void)
{
char *cur_ll;

if (cursor_to_ll != NULL)
	cur_ll = tparm (cursor_to_ll);
else
	cur_ll = tparm (cursor_address, lines-1, 0);

tputs (cur_ll, 1, cur_tput);
}

#ifdef TEST
main()
{
initscr();
refresh();
cur_goll();
getch();
endwin();
}
#endif
