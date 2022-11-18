/* mcelladr.c 1.8 1999/10/18 11:48:23 axel */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mc.h"
#include "mcutil.h"
#include "mcmisc.h"
#include "mcelladr.h"
#include "mcparse.h"

#ifndef TOGGLE
CELLPTR newcell() {
/* New empty cell */
CELLPTR	cp;

if ((cp = (CELLPTR)(malloc(sizeof(cellr)))) == NULL) return NULL;
cpcol(cp) = 0;
cprow(cp) = 0;
cpnext(cp) = NULL;
cptype(cp) = CONSTANT;
cpattrib(cp) = 0;
cpfor(cp) = DEFAULTFORMAT;
cptext(cp) = NULL;
cpvalue(cp) = .0;
cpcimag(cp) = .0;
cpunit(cp) = NULL;
return cp;
}

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
#endif

int colstring (int col, char *colstr)
/* Changes a column number to a string */
{
memset (colstr, 0, 3);
if (col < 26)
	{
	colstr[0] = col + 'A';
	return 1;
	}
else
	{
	colstr[0] = (col / 26) - 1 + 'A';
	colstr[1] = (col % 26) + 'A';
	return 2;
	}
} /* colstring */

#ifndef TOGGLE
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
#endif

char *creftoggle(char *fml, char *pos) {
int ln, ps, st, col, icol, row, irow;
char f[MAXINPUT];
char c[3];
char y;

ln = strlen(fml);
ps = pos - fml;
#define SEAR 0
#define INIT 1
#define ACOL 2
#define NCOL 3
#define RCOL 4
#define RROW 5
#define SEPA 6
#define CLOS 7
#define FINI 8
#define ERRO 9
if (ln > MAXINPUT || ps < 2 || ps > ln) return(pos);
/* parse the cell-address backwards with a state-machine */
st = SEAR;
col = icol = row = irow = 0;
#ifdef TOGGLE
  printf("\n0:ps:%d:y:%c:st:%d:col:%d:row:%d:icol:%d:irow:%d:\n", ps, fml[ps], st, col, row, icol, irow);
#endif
while (ps >= 0 && st != ERRO && st != FINI) {
  y = fml[ps];
#ifdef TOGGLE
  printf("\nA:ps:%d:y:%c:st:%d:col:%d:row:%d:icol:%d:irow:%d:\n", ps, y, st, col, row, icol, irow);
#endif
  switch(st) {
    case SEAR:
      if (isdigit(y) || y == ']') {
        ps += 2;
      } else {
        pos = fml + ps;
        st = INIT;
      }
      break;
    case INIT:
      if (y == ']') {
        st = RROW;
        row = 0;
        irow = 1;
      } else if (isdigit(y)) {
        st = RROW;
        row = y - '0';
        irow = 10;
      } else st = ERRO; break;
    case RROW:
      if (y == '+') {
        st = SEPA;
        irow = 1;
      } else if (y == '-') {
        st = SEPA;
        row *= -1;
        irow = 1;
      } else if (isdigit(y)) {
        row += (y - '0') * irow;
        irow *= 10;
      } else if (isalpha(y)) {
        irow = 0;
        col = toupper(y) - 'A' + 1;
        if (ps == 0) {
          st = FINI;
          icol = 0;
        } else {
          st = ACOL;
          icol = 26;
        }
      } else if (y == ',') {
        st = RCOL;
        irow = 0;
        col = 0;
        icol = 1;
      } else st = ERRO; break;
    case SEPA:
      if (y == ',') {
        st = RCOL;
        col = 0;
        icol = 1;
      } else st = ERRO; break;
    case RCOL:
      if (y == '+') {
        st = CLOS;
        icol = 1;
      } else if (y == '-') {
        st = CLOS;
        col *= -1;
        icol = 1;
      } else if (isdigit(y)) {
        col += (y - '0') * icol;
        icol *= 10;
      } else if (isalpha(y) && col == 0) {
        col = toupper(y) - 'A' + 1;
        if (ps == 0) {
          st = FINI;
          icol = 0;
        } else {
          st = ACOL;
          icol = 26;
        }
      } else if (y == '[') {
        st = FINI;
        icol = 0;
      } else st = ERRO; break;
    case ACOL:
      if (isalpha(y)) {
        col += (toupper(y) - 'A' + 1) * icol;
        if (ps == 0) {
          st = FINI;
          icol = 0;
        } else {
          st = ACOL;
          icol *= 26;
        }
      } else if (y == '[') {
        st = FINI;
        icol = 0;
      } else {
        st = FINI;
        icol = 0;
        ps += 1;
      } break;
    case CLOS:
      if (y == '[') {
        st = FINI;
      } else st = ERRO; break;
  }
#ifdef TOGGLE
  printf("B:ps:%d:y:%c:st:%d:col:%d:row:%d:icol:%d:irow:%d:\n", ps, y, st, col, row, icol, irow);
#endif
  if (st != FINI) ps -= 1;
}
if (st == FINI) {
  col += icol ? curcol : -1;
  row += irow ? currow : -1;
  strcpy(f, fml);
  colstring (col, c);
#ifdef TOGGLE
  printf(":%d:%s:%d:%d:%s:\n", icol*2 + irow, fml+ps, col, row, c);
#endif
  switch(icol*2 + irow) {
    case 0: sprintf(fml+ps, "[%+d,%+d]", col-curcol, row-currow); break;
    case 1: sprintf(fml+ps, "[%+d,%d]",  col-curcol, row+1);      break;
    case 2: sprintf(fml+ps, "[%s,%d]",   c,          row+1);      break;
    case 3: sprintf(fml+ps, "[%s,%+d]",  c,          row-currow); break;
  }
  ln = strlen(fml);
  strcat(fml, f+(pos - fml));
  return(fml + ln);
} else return(NULL);
}

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

#ifdef TOGGLE
int curcol = 2; /* C */
int currow = 2; /* 3 */

int main(argc, argv)
	int argc;
	char *argv[];
{
#ifdef COLS
int cl;
char cs[3];
for (cl=0; cl<110; cl++) {
  colstring (cl, cs);
  printf("%3d => %s\n", cl, cs);
}
#else
char formula[MAXINPUT];
char *np;

strcpy(formula, "1+b2+2");
printf(":%s:\n", formula);
np = creftoggle(formula, formula);
printf("0:%d:%s:\n-\n", (int)(np-formula), formula);
if (np!=formula || strcmp("1+b2+2", formula)) exit(1);

np = creftoggle(formula, formula+3);
printf("1:%d:%s:\n-\n", (int)(np-formula), formula);
if (np!=formula+9 || strcmp("1+[-1,-1]+2", formula)) exit(1);

np = creftoggle(formula, formula+9);
printf("2:%d:%s:\n-\n", (int)(np-formula), formula);
if (np!=formula+8 || strcmp("1+[B,-1]+2", formula)) exit(1);

np = creftoggle(formula, formula+8);
printf("3:%d:%s:\n-\n", (int)(np-formula), formula);
if (np!=formula+8 || strcmp("1+[-1,2]+2", formula)) exit(1);

np = creftoggle(formula, formula+8);
printf("4:%d:%s:\n-\n", (int)(np-formula), formula);
if (np!=formula+7 || strcmp("1+[B,2]+2", formula)) exit(1);

strcpy(formula, "1+AA102+2");
np = creftoggle(formula, formula+7);
printf("5:%d:%s:\n-\n", (int)(np-formula), formula);
if (np!=formula+11 || strcmp("1+[+24,+99]+2", formula)) exit(1);

strcpy(formula, "A2+2");
np = creftoggle(formula, formula+2);
printf("6:%d:%s:\n-\n", (int)(np-formula), formula);
if (np!=formula+7 || strcmp("[-2,-1]+2", formula)) exit(1);
#endif

exit(0);
}
#endif
