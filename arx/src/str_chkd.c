/* $Id: str_chkd.c,v 1.4 2015/07/01 13:31:13 axel Exp $
 */

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>

double str_chkd (const char *s)
{
double	d;
char	*ep;

if (*s == '\0') return 0.0;
errno = 0;
d = strtod (s, &ep);
/* not number if error code returned or	*/
/* pointer not at end of string		*/
return errno || *ep!='\0' ? HUGE_VAL : d;
}
