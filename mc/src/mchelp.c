/* $Id: mchelp.c,v 1.7 1999/10/24 05:58:08 axel Exp $
 */

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
