/* $Id: mcget.c,v 1.5 1997/05/09 17:35:25 axel Exp $
 */

#include <string.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcget.h"

int getkey (void)
{
return cur_getk (stdscr);
}

void clearinput (void)
/* Clears the input line */
{

#ifdef CURSESBUG
move (23, 79);
delch ();
mvaddch (inputline, 79, ' ');
#endif

move (inputline, 0);
clrtoeol();
} /* clearinput */

int editstringp (char *s, char *legal, int maxlength, int pos)
{
int	c;

c	= cur_gets (stdscr, inputline, 0, INPUTWIDTH, INPUTCOLOR,
		s, pos, legal, maxlength, NULL, creftoggle);
clearinput ();
return c == ESC ? 0 : c;
}

int editstring (char *s, char *legal, int maxlength)
{
return editstringp (s, legal, maxlength, 0);
}
