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

char *str_sub(char *s, int f, int l) {
/* count += (*s++ & 0xC0) != 0x80; */
char *tg;
char sv;
char *p;
char *q;
int n, m, o;
o = abs(l);
n = 0;
q = s;
for (p = s; *p; p++) {
  if (n == f) q = p;
  if (n >= f+o) break;
  n++;
}
n -= f;
sv = *p;
*p = '\0';
m = strlen(q);
if ((tg = malloc(m + 1 + o - n)) == NULL) return NULL;
if (l < 0) {
  memset(tg, ' ', o-n);
  strcpy(tg+o-n, q);
} else {
  strcpy(tg, q);
  memset(tg+m, ' ', o-n);
  tg[m+o-n] = '\0';
}
*p = sv;
return tg;
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

	while (NULL != gets(buf)) {
    buf[strlen(buf)-1] = '\0';
		printf("%d\n", str_tlen(buf));
		printf("2,2:%s:%s:\n", buf, str_sub(buf,2,2));
		printf("3,3:%s:%s:\n", buf, str_sub(buf,3,3));
		printf("0,7:%s:%s:\n", buf, str_sub(buf,0,7));
		printf("0,-7:%s:%s:\n", buf, str_sub(buf,0,-7));
		printf("0,9:%s:%s:\n", buf, str_sub(buf,0,9));
		printf("3,3333:%s:%s:\n", buf, str_sub(buf,3,3333));
  }
}
#endif
