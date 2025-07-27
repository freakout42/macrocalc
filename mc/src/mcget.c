/* mcget.c */

#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcget.h"
#include "mcolor.h"
#include "mcdisply.h"

int getkey (void)
{
printnull();
return cur_getk();
}

void clearinput (void)
/* Clears the input line */
{
move (inputline, 0);
clrtoeol();
} /* clearinput */

int editstringp (char *s, char *legal, int maxlength, int pos)
{
int	c;

c	= cur_gets (stdscr, inputline, 0, INPUTWIDTH, mcode2att(INPUTCOLOR),
		s, pos, legal, maxlength, NULL, creftoggle);
clearinput ();
return c == ESC ? 0 : c;
}

int editstring (char *s, char *legal, int maxlength)
{
return editstringp (s, legal, maxlength, 0);
}
