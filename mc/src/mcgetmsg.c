/* $Id: mcgetmsg.c,v 1.6 1995/05/10 05:08:41 axel Rel $
 */

#include <stdio.h>
#include <errno.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcgetmsg.h"

int getmessage (char *s, int n)
{
FILE	*msg;
char	msgcmd[MAXFILE*2+1];

sprintf (msgcmd, "%s/bin/mcmsg %d <%s/lib/mcmsg.rdb", libpath, n, libpath);

#ifdef DEBUG
fprintf (stderr, "getmsg: %s\n", msgcmd);
#endif

if ((msg = popen (msgcmd, "r")) == NULL) return RET_ERROR;
str_gets (msg, s, SCREENWIDTH+1);
if (errno==ESPIPE) errno = 0;
return pclose(msg);
}
