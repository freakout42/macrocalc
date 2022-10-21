/* $Id: str_gets.c,v 1.3 1995/02/24 09:25:11 axel Rel $
 */

#include <stdio.h>
#include <string.h>
#include <str_def.h>

char *str_gets (FILE *fp, char *s, size_t n)
{
register char	*es;

if (!fgets (s, n, fp)) return NULL;
es = s+strlen(s)-1;
if (*es=='\n') *es = '\0';
return s;
}

char *str_gecs (FILE *fp, char *s, size_t n)
{
register char	*es;

while ((es = str_gets (fp, s, n)) != NULL)
	if (*es != '#') return es;
return NULL;
}
