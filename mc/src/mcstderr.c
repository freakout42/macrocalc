/* $Id: mcstderr.c,v 1.6 1997/04/05 15:47:34 axel Exp $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <str_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcstderr.h"

char *getstderrmsg (char *m)
{
#ifdef STATPIPE
struct stat s;
#endif

if (std_erread == NULL) return NULL;

/* since fstat on pipes does not work on most systems,
 * don't use it any more
 */
#ifdef STATPIPE
fstat (fileno(std_erread), &s);
if (s.st_size == 0) return NULL;
#endif

return fgets (m, SCREENWIDTH+1, std_erread);
}
