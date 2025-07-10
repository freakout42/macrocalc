/* UTF-8 to ISO-8859-1/ISO-8859-15 mapper.
 * Return 0..255 for valid ISO-8859-15 code points, 256 otherwise.
 */
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

//# define size_t unsigned int

//include "oem2iso.h"

static int usednoniso;
unsigned int to_latin9(const unsigned int code)
{
    /* Code points 0 to U+00FF are the same in both. */
    if (code < 256U) return code;
    switch (code) {
    case 0x0152U:    return 188U; /* U+0152 = 0xBC: OE ligature */
    case 0x0153U:    return 189U; /* U+0153 = 0xBD: oe ligature */
    case 0x0160U:    return 166U; /* U+0160 = 0xA6: S with caron */
    case 0x0161U:    return 168U; /* U+0161 = 0xA8: s with caron */
    case 0x0178U:    return 190U; /* U+0178 = 0xBE: Y with diaresis */
    case 0x017DU:    return 180U; /* U+017D = 0xB4: Z with caron */
    case 0x017EU:    return 184U; /* U+017E = 0xB8: z with caron */
    case 0x20ACU:    return 164U; /* U+20AC = 0xA4: Euro */
    default: usednoniso = -1;
                     return 191U; /* U+00BF = 0xBF: ? inverted */
    }
}

unsigned int to_ucpoint(const unsigned int code)
{
    /* only ISO-8859-15 exceptions are converted */
    if (code < 128U || code > 256U) return code;
    switch (code) {
    case 188U:    return 0x0152U; /* U+0152 = 0xBC: OE ligature */
    case 189U:    return 0x0153U; /* U+0153 = 0xBD: oe ligature */
    case 166U:    return 0x0160U; /* U+0160 = 0xA6: S with caron */
    case 168U:    return 0x0161U; /* U+0161 = 0xA8: s with caron */
    case 190U:    return 0x0178U; /* U+0178 = 0xBE: Y with diaresis */
    case 180U:    return 0x017DU; /* U+017D = 0xB4: Z with caron */
    case 184U:    return 0x017EU; /* U+017E = 0xB8: z with caron */
    case 164U:    return 0x20ACU; /* U+20AC = 0xA4: Euro */
    default:      return code;
    }
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

#ifdef NONONO
/* Convert an UTF-8 string to ISO-8859-15.
 * All invalid sequences are ignored.
 * Note: output == input is allowed,
 * but   input < output < input + length
 * is not.
 * Output has to have room for (length+1) chars, including the trailing NUL byte.
 */
size_t utf8_to_latin9(char *output, const char *input, const size_t length)
{
    unsigned char             *out = (unsigned char *)output;
    const unsigned char       *in  = (const unsigned char *)input;
    const unsigned char *const end = (const unsigned char *)input + length;
    unsigned int               c;

    while (in < end)
        if (*in < 128)
            *(out++) = *(in++); /* Valid codepoint */
        else
        if (*in < 192)
            in++;               /* 10000000 .. 10111111 are invalid */
        else
        if (*in < 224) {        /* 110xxxxx 10xxxxxx */
            if (in + 1 >= end)
                break;
            if ((in[1] & 192U) == 128U) {
                c = to_latin9( (((unsigned int)(in[0] & 0x1FU)) << 6U)
                             |  ((unsigned int)(in[1] & 0x3FU)) );
                if (c < 256)
                    *(out++) = c;
            }
            in += 2;

        } else
        if (*in < 240) {        /* 1110xxxx 10xxxxxx 10xxxxxx */
            if (in + 2 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U) {
                c = to_latin9( (((unsigned int)(in[0] & 0x0FU)) << 12U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[2] & 0x3FU)) );
                if (c < 256)
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
                c = to_latin9( (((unsigned int)(in[0] & 0x07U)) << 18U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[3] & 0x3FU)) );
                if (c < 256)
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
                c = to_latin9( (((unsigned int)(in[0] & 0x03U)) << 24U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 18U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[3] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[4] & 0x3FU)) );
                if (c < 256)
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
                c = to_latin9( (((unsigned int)(in[0] & 0x01U)) << 30U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 24U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 18U)
                             | (((unsigned int)(in[3] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[4] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[5] & 0x3FU)) );
                if (c < 256)
                    *(out++) = c;
            }
            in += 6;

        } else
            in++;               /* 11111110 and 11111111 are invalid */

    /* Terminate the output string. */
    *out = '\0';

    return (size_t)(out - (unsigned char *)output);
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
#endif
