/* mctoggle.c */

#ifdef TEST
#include <stdio.h>
#endif
#include "mc.h"
#include <string.h>
#include <ctype.h>
#define MAXFORMULA 256

char *creftoggle(char *fml, char *pos) {
int ln, ps, st, col, icol, row, irow;
char f[MAXFORMULA];
char y;

ln = strlen(fml);
ps = pos - fml;
enum state {INIT,AROW,ACOL,NCOL,RCOL,RROW,SEPA,CLOS,FINI,ERRO};
if (ln > MAXFORMULA || ps < 2 || ps > ln) return(pos);
/* parse the cell-address backwards with a state-machine */
st = INIT;
while (ps > 0 && st != ERRO && st != FINI) {
  ps -= 1;
  y = fml[ps];
  switch(st) {
    case INIT:
      if (y == ']') {
        st = RROW;
        row = 0;
        irow = 1;
      } else if (isdigit(y)) {
        st = AROW;
        row = y - '0';
        irow = 10;
      } else st = ERRO; break;
    case RROW:
      if (y == '+' || y == '-') {
        st = SEPA;
        irow = 1;
      } else if (isdigit(y)) {
        row += (y - '0') * irow;
        irow *= 10;
      } else if (y == ',') {
        st = RCOL;
        irow = 0;
      } else st = ERRO; break;
    case AROW:
      if (isdigit(y)) {
        row += (y - '0') * irow;
        irow *= 10;
      } else if (isalpha(y)) {
        st = ACOL;
        irow = 0;
        col = toupper(y) - 'A';
        icol = 26;
      } else st = ERRO; break;
    case SEPA:
      if (y == ',') {
        st = RCOL;
        col = 0;
      } else st = ERRO; break;
    case RCOL:
      if (y == '+' || y == '-') {
        st = CLOS;
        icol = 1;
      } else if (isdigit(y)) {
        col += (y - '0') * icol;
        icol *= 10;
      } else if (isalpha(y) && col == 0) {
        st = ACOL;
        irow = 0;
        col = toupper(y) - 'A';
        icol = 26;
      } else if (y == '[') {
        st = FINI;
        icol = 0;
        ps -= 1;
      } else st = ERRO; break;
    case ACOL:
      if (isalpha(y)) {
        col += (toupper(y) - 'A') * icol;
        icol *= 26;
      } else if (y == '[') {
        st = FINI;
        col += 1;
        icol = 0;
      } else {
        st = FINI;
        col += 1;
        icol = 0;
        ps += 1;
      } break;
    case CLOS:
      if (y == '[') {
        st = FINI;
      } else st = ERRO; break;
  }
}
if (st == FINI) {
  strcpy(f, fml);
  sprintf(fml+ps, "[%d,%d]", col, row);
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
char formula[MAXFORMULA];
char *position;
char *np;

strcpy(formula, "1+b2+2");
position = formula;
printf(":%s:\n", formula);
np = creftoggle(formula, position);
printf(":%s:\n", formula);
if (np!=formula || strcmp("1+b2+2", formula)) exit(1);

np = creftoggle(formula, formula+4);
printf(":%d:%s:\n", np-formula, formula);
if (np!=formula+7 || strcmp("1+[2,2]+2", formula)) exit(1);

exit(0);
}
#endif
