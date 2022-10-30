/* $Id: mcelladr.c,v 1.8 1999/10/18 11:48:23 axel Exp $
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mc.h"
#include "mcutil.h"
#include "mcmisc.h"
#include "mcelladr.h"
#include "mcparse.h"

int coladr (char *s, int *col)
/* Returns 0 if the string is not a valid col address,
 * length of colstring otherwise.
 */
{
char *start = s;
long offset;
char *endp = NULL;

*col = 0;
if (*s=='[')
    {
    if (*(s+1)=='+' || *(s+1)=='-')
	{
	offset = strtol (s+1, &endp, 10);
	if (*endp!=',') return 0;
	*col = origcol + (int)offset + 1;
	s = endp;
	}
    else s++;
    }
while (isalpha(*s))
	{
	*col *= 26;
	*col += toupper(*s++) - 'A' + 1;
	}
*col -= 1;
if (*col>=MAXCOLS || *col<0) return 0;
if (endp!=NULL) *col = -*col-1;
return s-start;
}

int rowadr (char *s, int *row)
/* Returns 0 if the string is not a valid row address,
 * length of rowstring otherwise.
 */
{
char *start = s;
long offset;
char *endp = NULL;

*row = 0;
if (*s==',')
    {
    if (*(s+1)=='+' || *(s+1)=='-')
	{
	offset = strtol (s+1, &endp, 10);
	if (*endp!=']') return 0;
	*row = origrow + (int)offset + 1;
	s = endp;
	}
    else s++;
    }
while (isdigit(*s))
	{
	*row *= 10;
	*row += *s++ - '0';
	}
*row -= 1;
if (*row>=MAXROWS || *row<0) return 0;
if (*start==',' && *s++!=']') return 0;
if (endp!=NULL) *row = -*row-1;
return s-start;
}

int cellaadr (char *s, int *col, int *row)
/* Returns 0 if the string is not a valid cell address,
 * length of cellstring otherwise.
 * Also returns the column and row of the formula.
 */
{
int lc, lr;

if ((lc = coladr (s, col)) > 0 && (lr = rowadr (s+lc, row)) > 0)
	{
#ifdef DEBUG
fprintf (stderr, "cellaadr: %s : col=%d row=%d\n", s, *col, *row);
#endif
	return lc+lr;
	}
return 0;
}

int celladr (char *s, int *col, int *row)
/* Returns 0 if the string is not a valid cell address,
 * length of cellstring otherwise.
 * Also returns the column and row of the address.
 */
{
int l;
struct Range *r;

if ((l = cellaadr (s, col, row)) > 0)
	{
	*col = adrval(*col);
	*row = adrval(*row);
	return l;
	}
for (r = rnames; r != NULL; r = r->next)
	{
	l = strlen(r->name);
	if (!strncmp (r->name, s, l))
		{
		*col = r->adr[0].col;
		*row = r->adr[0].row;
		return l;
		}
	}
return 0;
}

int rangeadr (char *s, struct Range *range)
/* Returns 0 if the string is not a valid range address,
 * length of rangestring otherwise.
 * Also returns the columns and rows of the address.
 */
{
int l1, l2;
struct Range *r;

for (r = rnames; r != NULL; r = r->next)
	{
	l1 = strlen(r->name);
	if (!strncmp (r->name, s, l1))
		{
		*range = *r;
		return l1;
		}
	}
if ((l1 = celladr (s, &range->adr[0].col, &range->adr[0].row)) > 0 &&
    strchr("-.: ", *(s+l1)) != NULL &&
    (l2 = celladr (s+1+l1, &range->adr[1].col, &range->adr[1].row)) > 0) {
	if (range->adr[0].col > range->adr[1].col)
		swap (&range->adr[0].col, &range->adr[1].col);
	if (range->adr[0].row > range->adr[1].row)
		swap (&range->adr[0].row, &range->adr[1].row);
	return l1+1+l2;
	}
return 0;
}

char *celladrstring (int col, int row, char *s)
/* Changes a cell address to a string */
{
#ifdef DEBUG
char *start = s;
#endif

if (col<0 || row<0) {
    if (col<0) {
	sprintf (s, "[%+d", -col-1-origcol);
	s += strlen (s); }
    else {
	*s++ = '[';
	s += colstring (col, s); }
    if (row<0) {
	sprintf (s, ",%+d]", -row-1-origrow);
	s += strlen (s); }
    else {
	sprintf (s, ",%d]", row+1);
	s += strlen (s); }
    }
else	{
	s += colstring (col, s);
	sprintf (s, "%d", row+1);
	s += strlen (s);
	}
#ifdef DEBUG
fprintf (stderr, "celladrstring col=%d row=%d => %s\n", col, row, start);
#endif
return s;
} /* celladrstring */

#ifdef TEST
struct Range rg1 = {NULL, {0,0,0,0}, "hello"};
struct Range *rnames = &rg1;
int origcol = 20, origrow = 20;

main()
{
char s[80];
char t[80];
int len, col, row;

while (gets(s))
	{
	len = cellaadr (s, &col, &row);
	celladrstring (col, row, t);
	printf ("s=%s len=%d col=%d row=%d => %s\n", s, len, col, row, t);
	}
}
#endif
