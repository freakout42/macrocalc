/* mchelp.c */

#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include "mc.h"
#include "mcommon.h"
#include "mcput.h"
#include "mchelp.h"
#include "mcdisply.h"

void mchelp (void)
{
#ifdef WIN32
char helpcmd[256] = "start ";
strcat(helpcmd, "https://mc.freakout.de/mc1.html");
#else
char helpcmd[256] = "man ";
strcat(helpcmd, progname);
#endif
mcsystem (helpcmd, FALSE);
}
