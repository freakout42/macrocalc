/* UTF-8 to ISO-8859-1/ISO-8859-15 mapper.
 * Return 0..255 for valid ISO-8859-15 code points, 256 otherwise.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "utf8nfc.h"

#define MAXMAPS 32
static int usednoniso;
static int nonisofree = 0;

static unsigned int mapnfc (char bas, int cod) {
struct dmap *dcy;
if (cod >= 0x0300 && cod <= 0x0338 && bas >= 'A' && bas <= 'z')
  for (dcy = diacry; bas >= dcy->basech; dcy++)
    if (bas == dcy->basech && cod == dcy->nonsp) return dcy->iso15;
return cod;
}

static int iso2ucodet[128] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0xa0,0xa1,0xa2,0xa3,0x20ac,0xa5,0x0160,0xa7,0x0161,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
  0xb0,0xb1,0xb2,0xb3,0x017d,0xb5,0xb6,0xb7,0x017e,0xb9,0xba,0xbb,0x0152,0x0153,0x0175,0xbf,
  0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
  0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
  0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
  0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff
};

int iso2ucode(unsigned char cod) {
  return cod < 128U ? cod : iso2ucodet[cod - 128];
}

int utf162utf8(char* out, int c) {
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

int to_utf8(char *buf, int nbuf) {
  char *buf2;
  char *src;
  char *tgt;
  buf2 = src = malloc(nbuf + 1);
  strncpy(buf2, buf, nbuf);
  tgt = buf;
  while ((src - buf2) < nbuf) tgt += utf162utf8(tgt, iso2ucode(*src++));
  free(buf2);
  return tgt - buf;
}

int to_latin9(const unsigned int code) {
  int i;
  if (code < 256U) return code;
  for (i=0; i < 128; i++) if (iso2ucodet[i] == code) { return i + 128; }
  if (nonisofree < MAXMAPS) { iso2ucodet[nonisofree++] = code; return nonisofree + 127; }
  else { usednoniso = -1; return 191U; } /* U+00BF = 0xBF: ? inverted */
}

/* Convert an UTF-8 string to ISO-8859-15 */
size_t utf8_to_latin9(unsigned char *output, unsigned char *input, size_t length) {
    unsigned char             *out = (unsigned char *)output;
    const unsigned char       *in  = (const unsigned char *)input;
    const unsigned char *const end = (const unsigned char *)input + length;
    unsigned int               c; /* current */
    unsigned int               u; /* upoint */
    unsigned int               v; /* normalized */
    unsigned char              l; /* last */

    l = '\0';
    while (in <= end) {
        c = '\0';
        if (*in < 128)
            c = *in;
        else
        if (*in < 192)
            ;               /* 10000000 .. 10111111 are invalid */
        else
        if (*in < 224) {        /* 110xxxxx 10xxxxxx */
            if (in + 1 >= end)
                ;
            else
            if ((in[1] & 192U) == 128U) {
                u =            (((unsigned int)(in[0] & 0x1FU)) << 6U)
                             |  ((unsigned int)(in[1] & 0x3FU))  ;
                v = mapnfc (l, u);
                if (v != u) l =          ( v ); else c =          ( u );
            }
            in += 1;

        } else
        if (*in < 240) {        /* 1110xxxx 10xxxxxx 10xxxxxx */
            if (in + 2 >= end)
                ;
            else
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U) {
                c =          ( (((unsigned int)(in[0] & 0x0FU)) << 12U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[2] & 0x3FU)) );
            }
            in += 2;

        } else
        if (*in < 248) {        /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 3 >= end)
                ;
            else
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U) {
                c =          ( (((unsigned int)(in[0] & 0x07U)) << 18U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[3] & 0x3FU)) );
            }
            in += 3;

        } else
        if (*in < 252) {        /* 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 4 >= end)
                ;
            else
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U) {
                c =          ( (((unsigned int)(in[0] & 0x03U)) << 24U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 18U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[3] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[4] & 0x3FU)) );
            }
            in += 4;

        } else
        if (*in < 254) {        /* 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 5 >= end)
                ;
            else
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U &&
                (in[5] & 192U) == 128U) {
                c =          ( (((unsigned int)(in[0] & 0x01U)) << 30U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 24U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 18U)
                             | (((unsigned int)(in[3] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[4] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[5] & 0x3FU)) );
            }
            in += 5;

        } else
            ;

        if (l) *(out++) = to_latin9(l);
        l = to_latin9(c);
        in++;
    }
    *out = '\0';

    return (size_t)(out - (unsigned char *)output);
}

int to_utf16(char *buf, int nbuf) {
  char *buf2;
  int len;
  buf2 = strdup(buf);
  usednoniso = 1;
  len = utf8_to_latin9((unsigned char *)buf, (unsigned char *)buf2, strlen(buf2));
  free(buf2);
  return len * usednoniso;
}

#ifdef LIBVERSION
int to_utf8(char *buf, int nbuf) {
  wchar_t *se;
  int len, i;
  se = malloc((nbuf+2) * sizeof(wchar_t));
  for (i=0; i<nbuf; i++) se[i] = to_ucpoint((unsigned char)buf[i]);
                         se[i] = '\0';
  len = wcstombs(buf, se, nbuf*2);
  free(se);
  return len;
}

int to_utf16(char *buf, int nbuf) {
  wchar_t *se;
  int i, len;
  se = malloc((nbuf+2) * sizeof(wchar_t));
  len = mbstowcs(se, buf, nbuf+2);
  usednoniso = 1;
  for (i=0; i<=len; i++) buf[i] = to_latin9(se[i]);
  free(se);
  return len * usednoniso;
}
#endif
