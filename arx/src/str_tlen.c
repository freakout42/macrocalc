/* $Id: str_tlen.c,v 1.1 2015/02/04 11:34:13 axel Exp $
 * length without trailing whitespace
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <str_def.h>

int str_tlen (char *s)
{
register char *p;

if (NULL == (p = strchr(s, '\0')))
	return (0);
for (; isspace(*s); s++);
for (--p; (p > s) && isascii(*p) && isspace(*p);)
	p--;
return (p-s+1);
}

#ifdef TEST
main()
{
	char buf[80];

	while (NULL != gets(buf))
		printf("%d\n", str_tlen(buf));
}
#endif
