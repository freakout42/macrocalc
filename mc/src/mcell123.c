/* mcell123.c 1.6 2001/11/07 12:23:50 axel */

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <arx_def.h>
#include "mc.h"
#include "lotus.h"
#include "mcelladr.h"

char *celladrs123 (CELLADR123 *a, char *s)
/* Changes a lotus cell adress to a string */
{
strcpy (s++, "[");
if (a->col>=0)
	{
	s += colstring (a->col, s);
	}
else
	{
	a->col &= 0x7fff;
	if (a->col & 0x2000) a->col |= 0xd000;
	sprintf (s, "%+d", a->col);
	s += strlen (s);
	}
*s++ = SEPCH;
*s = '\0';;
if (a->row>=0)
	{
	sprintf (s, "%d]", a->row+1);
	}
else
	{
	a->row &= 0x7fff;
	if (a->row & 0x2000) a->row |= 0xd000;
	sprintf (s, "%+d]", a->row);
	}
s += strlen (s);
return s;
}
