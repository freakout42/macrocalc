/* str_tlen.c
 * UTF-8 to ISO-8859-1/ISO-8859-15 mapper.
 * Return 0..255 for valid ISO-8859-15 code points, 256 otherwise.
 * length without trailing whitespace
 * substring with printf %*.*s capabilities from utf-8 mbstowcs
 * cc -I../inc -DTEST str_tlen.c -olll
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>
#include "oem2iso.h"

char *arx_vers(char *rcs) {
#ifdef UTF8
static char version[132];
snprintf(version, 130, "%-.72s w=%c", rcs, cur_utf8 ? '1' : '0');
return version;
#else
return rcs;
#endif
}

void oem858_to_iso885915(char *output)
{
  while (*output) {
    *output = oem858[(unsigned char)(*output)];
    output++;
  }
}

void iso885915_to_oem858(char *output)
{
  while (*output) {
    *output = iso885915[(unsigned char)(*output)];
    output++;
  }
}

int cur_utf8 = 0;

/* Convert an UTF-8 string to UCODE
 * All invalid sequences are ignored.
 * Note: output == input is allowed,
 * but   input < output < input + length is not.
 * Output has to have room for (length+1) chars, including the trailing NUL byte.
 */
int utf8_to_ucode(wchar_t *output, char *input, int length) {
    wchar_t                   *out = output;
    const unsigned char       *in  = (const unsigned char *)input;
    const unsigned char *const end = (const unsigned char *)input + strlen(input);
    unsigned int               c;

    while (in < end)
        if (*in < 128)
            *(out++) = *(in++); /* Valid codepoint */
        else
        if (*in < 192)
            break;
/*          in++;                * 10000000 .. 10111111 are invalid */
        else
        if (*in < 224) {        /* 110xxxxx 10xxxxxx */
            if (in + 1 >= end)
                break;
            if ((in[1] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x1FU)) << 6U)
                    | ((unsigned int)(in[1] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 2;

        } else
        if (*in < 240) {        /* 1110xxxx 10xxxxxx 10xxxxxx */
            if (in + 2 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x0FU)) << 12U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[2] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 3;

        } else
        if (*in < 248) {        /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 3 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x07U)) << 18U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 12U)
                    | (((unsigned int)(in[2] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[3] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 4;

        } else
        if (*in < 252) {        /* 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 4 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x03U)) << 24U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 18U)
                    | (((unsigned int)(in[2] & 0x3FU)) << 12U)
                    | (((unsigned int)(in[3] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[4] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 5;

        } else
        if (*in < 254) {        /* 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 5 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U &&
                (in[5] & 192U) == 128U) {
                c =   (((unsigned int)(in[0] & 0x01U)) << 30U)
                    | (((unsigned int)(in[1] & 0x3FU)) << 24U)
                    | (((unsigned int)(in[2] & 0x3FU)) << 18U)
                    | (((unsigned int)(in[3] & 0x3FU)) << 12U)
                    | (((unsigned int)(in[4] & 0x3FU)) << 6U)
                    |  ((unsigned int)(in[5] & 0x3FU)) ;
                *(out++) = c;
            }
            in += 6;

        } else
            in++;               /* 11111110 and 11111111 are invalid */

    /* Terminate the output string. */
    *out = 0;

    if (in < end) return -1;
    else          return out - output;
}

static int utf162utf8(char* out, int c) {
int bits;
char *out1;
out1 = out;
if      (c <    0x80) {  *out++=  c;                         bits = -6; }
else if (c <   0x800) {  *out++= ((c >>  6) & 0x1F) | 0xC0;  bits =  0; }
else if (c < 0x10000) {  *out++= ((c >> 12) & 0x0F) | 0xE0;  bits =  6; }
else                  {  *out++= ((c >> 18) & 0x07) | 0xF0;  bits = 12; }
for ( ; bits >= 0; bits -= 6) *out++= ((c >> bits) & 0x3F) | 0x80;
return out - out1;
}

int str_w2u8(char *tgt, wchar_t *src, int siz) {
char *t;
wchar_t *s;
t = tgt;
for (s = src; *s; s++) t += utf162utf8(t, *s);
*t = '\0';
return t - tgt;
}

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

str_sub(out, "", 0, 3, 0);
printf("str_sub(out, \"\", 0, 3, 0) = :%s:\n", out);

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
