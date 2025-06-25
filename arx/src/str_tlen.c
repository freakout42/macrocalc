/* $Id: str_tlen.c,v 1.1 2015/02/04 11:34:13 axel Exp $
 * length without trailing whitespace
 * substring with printf %*.*s capabilities from utf-8 mbstowcs
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>

int cur_utf8 = 0;

char *str_sub(char *s, int f, int l, int z) {
char *tg;
char sv = '\0';
char *p;
char *q;
int n, m, o, r;
o = abs(l);
n = 0;
r = strlen(s);
q = s + r;
for (p = s; *p; p++) {
 if (!cur_utf8 || (*p & 0xC0) != 0x80) {
  if (n == f) q = p;
  if (z > 0 && n >= f+z) break;
  n++;
 }
// fprintf(stderr,"%d %c %4x %4x %d\n",(!cur_utf8),*p,*p & 0xC0,0x80,n);
}
n -= f;
if (z > 0) {
  sv = *p;
  *p = '\0';
}
m = strlen(q);
if ((tg = malloc(m + 1 + o + r)) == NULL) return NULL;
if (o-n > 0) {
 if (l < 0) {
  memset(tg, ' ', o-n);
  strcpy(tg+o-n, q);
 } else {
  strcpy(tg, q);
  memset(tg+m, ' ', o-m);
  tg[m+o-m] = '\0';
 } } else {
  strcpy(tg, q);
}
if (z > 0) *p = sv;
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
	char *out;

cur_utf8 = 1;

#define testsub(bf, fi, li, zi, sb) \
strcpy(buf, #bf); \
out = str_sub(buf, fi, li, zi); \
if (strcmp(out, sb)) printf(#fi ", " #li ", " #zi " failed with :%s: -> :%s:\n", buf, out); \
free(out)

testsub(012345, 2,  2,  0, "2345"            );
testsub(012345, 2,  7,  0, "2345   "         );
testsub(012345, 2, -7,  0, "   2345"         );
testsub(012345, 2,  2,  3, "234"             );
testsub(012345, 5,  2,  3, "5 "              );
testsub(012345, 5,  0,  3, "5"               );
testsub(012345, 6,  0,  3, ""                );
testsub(012345, 6,  4,  3, "    "            );
testsub(0123,   2, -4,  0, "  23"            );
testsub(metric,11, 16, 16, "                ");
testsub(hallochen1234567890123456789,0,10,10, "hallochen1");
testsub(hall£chen1234567890123456789,0,10,10, "hall£chen1");
}
#endif
