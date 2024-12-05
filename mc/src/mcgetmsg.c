/* mcgetmsg.c */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcgetmsg.h"

int getmessage (char *s, int n)
{
#ifdef MSGINLINE
#define makemsg(n,s) #s,
static char *mcmsg[] = {
#include "mcmsg.h"
""};     
strcpy(s, mcmsg[n]);
return 0;
#else
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
#endif
}
