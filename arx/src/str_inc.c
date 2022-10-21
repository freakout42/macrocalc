/* $Id: str_inc.c,v 1.5 1995/02/24 09:26:13 axel Rel $
 */

#define MEMMOVE
#include <stdio.h>
#include <string.h>
#include <str_def.h>

char *str_inc (char *s, const char ic)
{
#ifdef MEMMOVE
memmove (s+1, s, strlen(s)+1);
*s = ic;
#else
register char	*ss;

for (ss = s + strlen (s); ss >= s; ss--)
	*(ss+1)	= *ss;
*(ss+1)	= ic;
#endif
return s;
}

char *str_ins (char *s, char *i)
{
char	tmp[256];
char	*sd;

strcpy (tmp, s);
strcpy (s, i);
sd = s + strlen(i);
strcat (s, tmp);
return sd;
}
