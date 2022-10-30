/* mctoggle.c */

#ifdef TEST
#include <stdio.h>
#endif
#include <string.h>
#include <ctype.h>
#include "mc.h"
#define MAXFORMULA 256

#ifdef TEST
int curcol = 2; /* C */
int currow = 2; /* 3 */
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

char *creftoggle(char *fml, char *pos) {
int ln, ps, st, col, icol, row, irow;
char f[MAXFORMULA];
char c[3];
char y;

ln = strlen(fml);
ps = pos - fml;
enum state {INIT,ACOL,NCOL,RCOL,RROW,SEPA,CLOS,FINI,ERRO};
/*          0    1    2    3    4    5    6    7    8 */
if (ln > MAXFORMULA || ps < 2 || ps > ln) return(pos);
/* parse the cell-address backwards with a state-machine */
st = INIT;
col = icol = row = irow = 0;
#ifdef TEST
  printf("\n0:ps:%d:y:%c:st:%d:col:%d:row:%d:icol:%d:irow:%d:\n", ps, fml[ps], st, col, row, icol, irow);
#endif
while (ps > 0 && st != ERRO && st != FINI) {
  ps -= 1;
  y = fml[ps];
#ifdef TEST
  printf("\nA:ps:%d:y:%c:st:%d:col:%d:row:%d:icol:%d:irow:%d:\n", ps, y, st, col, row, icol, irow);
#endif
  switch(st) {
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
        st = ACOL;
        irow = 0;
        col = toupper(y) - 'A' + 1;
        icol = 26;
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
        st = ACOL;
        col = toupper(y) - 'A' + 1;
        icol = 26;
      } else if (y == '[') {
        st = FINI;
        icol = 0;
        ps -= 1;
      } else st = ERRO; break;
    case ACOL:
      if (isalpha(y)) {
        col += (toupper(y) - 'A' + 1) * icol;
        icol *= 26;
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
#ifdef TEST
  printf("B:ps:%d:y:%c:st:%d:col:%d:row:%d:icol:%d:irow:%d:\n", ps, y, st, col, row, icol, irow);
#endif
}
if (st == FINI) {
  col += icol ? curcol : -1;
  row += irow ? currow : -1;
  strcpy(f, fml);
  colstring (col, c);
#ifdef TEST
  printf(":%d:%s:%d:%d:%s:\n", icol*2 + irow, fml+ps, col, row, c);
#endif
  switch(icol*2 + irow) {
    case 0: sprintf(fml+ps, "[%+d,%+d]", col-curcol, row-currow); break;
    case 1: sprintf(fml+ps, "[%+d,%d]", col-curcol, row+1);       break;
    case 2: sprintf(fml+ps, "[%s,%d]", c, row+1);                 break;
    case 3: sprintf(fml+ps, "[%s,%+d]", c, row-currow);           break;
  }
  ln = strlen(fml);
  strcat(fml, f+(pos - fml));
  return(fml + ln);
} else return(NULL);
}

#ifdef TEST
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
char formula[MAXFORMULA];
char *np;

strcpy(formula, "1+b2+2");
printf(":%s:\n", formula);
np = creftoggle(formula, formula);
printf("0:%d:%s:\n-\n", np-formula, formula);
if (np!=formula || strcmp("1+b2+2", formula)) exit(1);

np = creftoggle(formula, formula+4);
printf("1:%d:%s:\n-\n", np-formula, formula);
if (np!=formula+9 || strcmp("1+[-1,-1]+2", formula)) exit(1);

np = creftoggle(formula, formula+9);
printf("2:%d:%s:\n-\n", np-formula, formula);
if (np!=formula+8 || strcmp("1+[B,-1]+2", formula)) exit(1);

np = creftoggle(formula, formula+8);
printf("3:%d:%s:\n-\n", np-formula, formula);
if (np!=formula+8 || strcmp("1+[-1,2]+2", formula)) exit(1);

np = creftoggle(formula, formula+8);
printf("4:%d:%s:\n-\n", np-formula, formula);
if (np!=formula+7 || strcmp("1+[B,2]+2", formula)) exit(1);

strcpy(formula, "1+AA102+2");
np = creftoggle(formula, formula+7);
printf("5:%d:%s:\n-\n", np-formula, formula);
if (np!=formula+11 || strcmp("1+[+24,+99]+2", formula)) exit(1);
#endif

exit(0);
}
#endif
