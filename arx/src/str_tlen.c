/* $Id: str_tlen.c,v 1.1 2015/02/04 11:34:13 axel Exp $
 * length without trailing whitespace
 mbstowcs
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>

int *str_offi(char *s) {
/*
    size_t count = 0;
    while (*s) {
        count += (*s++ & 0xC0) != 0x80;
    }
    return count;
 */
int *offs;
int *p;
char *q;
int i, j;
j = strlen(s);
if (j >= MAXSTRING || ((offs = malloc(sizeof(int *)*j)) == NULL)) return NULL;
for (p = offs, q = s; *q; p++) {
  i = 1;
}
return p;
}

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
