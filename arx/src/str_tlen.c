/* str_tlen.c
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

int str_pos(char *s, int f) {
char *p;
int n, m;
n = 0;
m = 0;
for (p = s; *p; p++) {
  if (m++ >= f) break;
  if (!cur_utf8 || (*p & 0xC0) != 0x80) n++;
  }
return n;
}

/* s iso or utf-8 (cur_utf8=TRUE) char[]
 * f from pos
 * l min size padded <0 right align
 * z max length
 */
char *str_sub(char *tg, char *s, int f, int l, int z) {
char sv = '\0';
char *p;
char *q;
int n, m, v, o, r;
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
v = str_pos(q, m);
if (tg==NULL && ((tg = malloc(m + 1 + o + r)) == NULL)) return NULL;
if (o-n > 0) {
 if (l < 0) {
  memset(tg, ' ', o-n);
  strcpy(tg+o-n, q);
 } else {
  strcpy(tg, q);
  memset(tg+m, ' ', o-v);
  tg[m+o-v] = '\0';
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
  int pos;

cur_utf8 = 1;

#define testsub(bf, fi, li, zi, sb) \
strcpy(buf, #bf); \
out = str_sub(NULL, buf, fi, li, zi); \
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
testsub(1£2,0,10,10, "1£2       ");

#define testpos(bf, fi, li) \
strcpy(buf, #bf); \
pos = str_pos(buf, fi); \
if (pos != li) printf(#fi ", " #li " failed with :%s: %d\n", buf, pos);

testpos(hall£chen10,10,9);
testpos(hall£chen10,0,0);
testpos(hall£chen10,2,2);
testpos(hall£chen10,12,11);
}
#endif
