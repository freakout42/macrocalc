/* $Id: str_trim.c,v 1.4 2015/02/04 11:34:13 axel Exp $
 * Remove trailing whitespace spaces from a line.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <str_def.h>

char *str_trim (char *s)
{
register char *p;

if (NULL == (p = strchr(s, '\0')))
	return (NULL);
for (; isspace(*s); s++);
for (--p; (p > s) && isascii(*p) && isspace(*p);)
	*p-- = '\0';
return (s);
}

#ifdef TEST
main()
{
	char buf[80];

	while (NULL != gets(buf))
		printf("'%s'\n", str_trim(buf));
}
#endif
