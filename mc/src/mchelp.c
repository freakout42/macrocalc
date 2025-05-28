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
char helpcmd[256];
#ifdef WIN32
strcpy(helpcmd, "start https://mc.freakout.de/mc1.html");
#else
/* Fixes #16 */
snprintf(helpcmd, 256, "man %s/man/man1/%s.1", libpath, progname);
#endif
mcsystem (helpcmd, FALSE);
}
