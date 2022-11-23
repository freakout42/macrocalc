/* mconvert.c 1.10 2008/06/11 09:15:43 axel
 * convert format
 * convert label with tab[]
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include "lotus.h"
#include "lics.h"
#include "mc2wks.h"
#include "mcelldef.h"
#include "mcparse.h"

unsigned char convertlformat (unsigned char form) {
switch (form & FORMATM) {
  case FIXED:   form = (form & (0xffu ^ FORMATM)) | L_FIXED;   break;
  case SPECIAL: form = (form & (0xffu ^ FORMATM)) | L_SPECIAL; break;
}
return form;
}

unsigned char convertformat (unsigned char format)
{
switch (format & FORMATM)
 {
 case L_FIXED:
 case SCIENTIFIC:
 case CURRENCY:
 case PERCENT:
 case COMMA:
	return (format);
 case L_SPECIAL:
	switch (format & PLACES)
	 {
	 case HIDDEN:
	 case DATE:
	 case TIME:
		return (format);
	 case BAR:
	 case TEXTF:
	 case GENERAL:
		return ((format & PROTECT) | DEFAULTFORMAT);
	 case DAYMONTH:
	 case MONTHYEAR:
	 case DATEI1:
	 case DATEI2:
		return ((format & (PROTECT | FORMATM)) | DATE);
	 case HOURMIN:
	 case TIMEI1:
	 case TIMEI2:
		return ((format & (PROTECT | FORMATM)) | TIME);
	 case L_DEFAULT:
		return (DEFAULTFORMAT);
	 }
 }
return (DEFAULTFORMAT);
}

extern int	warn;
extern int	erro;

void convertlabel (Lotrec *r)
{
unsigned char	*str;		/* string to convert		*/
unsigned char	buf[255];	/* buffer for conversion	*/
unsigned char	*b	= buf;	/* pointer for write into buffer*/
unsigned char	*s;		/* pointer to conversion string	*/
#ifdef IBMCODE
int		i;		/* tab array pointer		*/
#endif

/* check if string is valid	*/
s = (unsigned char*)&r->data;
if (*(s+r->reclen-1) != '\0')	fatal (12, "Label not null terminated.");

/* convert all chars in str	*/
for (str = (unsigned char*)r->data.label.s; *str; str++)
	{
#ifdef IBMCODE
	if (*str<128)
#endif
		*b++ = *str;	/* char is ascii		*/
#ifdef IBMCODE
	else if (*tab[i=*str-128]>127)
		*b++ = *tab[i];	/* char is in both tables	*/
	else if (tab[i])
		/* char is not in target table, take string	*/
		{
		for (s=tab[i]; *s; *b++ = *s++)
		warn	= 1;
		}
	else
		erro = 1;	/* char is not valid		*/
#endif
	}

/* copy buffer into string and fix reclen	*/
*b	= '\0';
strcpy (r->data.label.s, (char*)buf);
r->reclen	= r->data.label.s - (char*)&r->data
		+ strlen (r->data.label.s) + 1;
}

static short mc2lotus (int ma, int orig)
/* lotus decodes absolute address as positive value of col or row offset 0
 * relative address is flaged with 0x8000 (negative bit)
 * and cut with 0x3fff
 */
{
short la;

la = (short) adrval(ma);
if (ma<0) {
	la -= orig;
	la &= 0x3fff;
	la |= 0x8000;
	}
lib_cano(la);
return la;
}

void convertcelladr (CELLADR123 *ca123, struct CELLADR *ca)
{
#ifdef DEBUG
fprintf (stderr, "convertcelladr: <col=%d <row=%d\n", ca->col, ca->row);
#endif
ca123->col = mc2lotus (ca->col, origcol);
ca123->row = mc2lotus (ca->row, origrow);
#ifdef DEBUG
fprintf (stderr, "convertcelladr: >col=%d >row=%d\n", ca123->col, ca123->row);
#endif
}
