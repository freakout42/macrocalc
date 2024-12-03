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
char helpcmd[256] = "man ";

strcat(helpcmd, progname);
mcsystem (helpcmd, FALSE);
}
