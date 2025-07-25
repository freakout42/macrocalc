/* mcmessag.c */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcput.h"
#include "mcget.h"
#include "mcutil.h"
#include "mcmisc.h"
#include "mcgetmsg.h"
#include "mcmessag.h"
#include "mcolor.h"

void errorprint (char *format, ...) {
va_list args;
char s[MAXINPUT+1];

va_start(args, format);
vsnprintf(s, MAXINPUT, format, args);
va_end(args);
errorstr(s);
}

void smessage (char *s)
/* Prints a string-message */
{
rmendnl (s);
writef (0, errorline, PROMPTCOLOR, SCREENWIDTH, s);
move (errorline, 0);
} /* message */

void dmessage (char *s)
/* Displays a string-message */
{
smessage (s);
cur_refr();
} /* message */

void message (int n)
/* Prints a message */
{
char m[MAXSCREENWIDTH+1];

getmessage (m, n);
dmessage (m);
} /* message */

void errorstr (char *s)
/* Prints an error line */
{
int maxl;

rmendnl (s);
beep();      /* Beeps the speaker */
maxl = SCREENWIDTH - strlen(TXTKEYPRESS) - 1;
writef (0, errorline, MESSAGECOLOR, SCREENWIDTH, "%-.*s %s", maxl, s, TXTKEYPRESS);
move (errorline, 0);
cur_getp (stdscr);
clrtoeol();
} /* errorstr */

void errormsg (int n)
/* Prints an error message */
{
char m[MAXSCREENWIDTH+1];

if (!getmessage (m, n)) errorstr (m);
} /* errormsg */

void writeprompt (int prompt)
/* Prints a prompt on the screen */
{
char m[MAXSCREENWIDTH+1];

getmessage (m, prompt);
rmendnl (m);
writef (0, errorline, PROMPTCOLOR, SCREENWIDTH, m);
move (errorline, strlen(m));
} /* writeprompt */

void clearprompt (void)
{
/*mvaddch (errorline, 79, ' ');*/
move (errorline, 0);
clrtoeol();
}
