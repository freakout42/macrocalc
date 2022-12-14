/* $Id: cur_goll.c,v 1.6 1999/10/14 18:21:57 axel Exp $
 * curses goto lower left
 */

#include <stdio.h>
#include <cur_def.h>
#include <term.h>

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
