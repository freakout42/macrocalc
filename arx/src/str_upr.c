/* $Id: str_upr.c,v 1.6 2006/09/27 08:23:58 axel Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <arx_def.h>
#include <str_def.h>

char *str_upr (char *s)
{
#ifdef _AIX
/* weird behaviour of aix's cc */
register unsigned char	*ss, *st;
#else
register char	*ss, *st;
#endif

ss = st	= s;
for (; *ss; ss++)
	{
	if (islower(*ss)) *st++ = toupper (*ss);
	else if (isspace(*ss) || isprint(*ss) ||
#ifdef _AIX
/* weird behaviour of aix's cc */
						 *ss>128
#else
						 *ss<0
#endif
							) *st++ = *ss;
	}
*st = '\0';
return s;
}

char *str_low (char *s)
{
#ifdef _AIX
/* weird behaviour of aix's cc */
register unsigned char	*ss, *st;
#else
register char	*ss, *st;
#endif

ss = st	= s;
for (; *ss; ss++)
	{
	if (isupper(*ss)) *st++ = tolower (*ss);
	else if (isspace(*ss) || isprint(*ss) ||
#ifdef _AIX
/* weird behaviour of aix's cc */
						 *ss>128
#else
						 *ss<0
#endif
							) *st++ = *ss;
	}
*st = '\0';
return s;
}
