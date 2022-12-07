/* mcparse.c */

#ifdef DEBUG
#include <stdio.h>
#endif

#include <assert.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <arx_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcelladr.h"
#include "mcparse.h"

extern int 	yydebug;
int		yybegin();
int		yyparse();
void		yysetbuf();
void		yyclrbuf();

#ifndef LOTUS
int origcol, origrow; /* original column */
int errpos; /* position of parsing error */
CELLPTR pc;
char *yybuf;
char *yysparse; /* pointer to source formula */
char *yybparse; /* pointer to parsed formula */
char *yytoparse; /* pointer to left formula */
int yyerrorflg; /* error flag for lex-parser */
#endif

/* build parse string with references embedded */
static int gettoparse (char *err)
{
int		col, row;
CELLPTR		cellptr;
int		len;
char		*s;
char		*t;

s = yysparse;
t = yybparse;
do	{
	if ( *s == '&' &&
	     (len = celladr(s+1, &col, &row)) &&
	     (cellptr = cell(col, row)) )
		{
		strcpy(t, cptextstr(cellptr));
		s += len+1;
		t += strlen(t);
		}
	else	*t++ = *s;
	if (err != NULL && err <= t) break;
	} while (*s++);
return s - yysparse;
}

/* Parses the cell c */
#ifdef LOTUS
int parse2 (CELLPTR c, short *formula)
/* Returns the formula of the evaluated string */
#else
int parse (CELLPTR c, char *parsed)
/* Parses into the cell c and parsed formula */
#endif

{
#ifdef LOTUS
char *polform;
short len;
#else
char yybuffer[MAXPARSED+1];
#endif
char yyinput[MAXPARSED+1];

pc = c;
cptype(pc) = CONSTANT;
origcol = cpcol(pc);
origrow = cprow(pc);
yysparse = cptext(c);
yybparse = yytoparse = yyinput;
yyerrorflg = 0;
gettoparse(NULL);

#ifndef LOTUS
if (parsed!=NULL) strcpy(parsed, yybparse);
yybuf = yybuffer;
*yybuffer = '\0';

#else
yybuf = polform = (char*)(formula + 1);

#endif
yysetbuf(yybparse);

errno = 0;
#ifdef YYDEBUG
yydebug = 1;
#endif

yybegin();
yyparse();
if (cpunit(pc) && strspn(cpunit(pc), " ")==strlen(cpunit(pc))) cpunit(pc) = NULL;
if (cptype(pc) == DATETYPE) {
  cptype(pc) = CONSTANT;
  if (cpfor(pc) == 0) cpfor(pc) = SPECIAL|DATE;
}
yyclrbuf();

#ifdef LOTUS
len = yybuf - polform;
memcpy(formula, &len, sizeof(short));
return cptype(pc);
#else
return errno;
#endif
} /* parse */

#ifndef LOTUS
int mcinput()
{
register int	c = ' ';

#ifdef DEBUG
fprintf(stderr, "input: %s\n", yytoparse);
#endif

c = *yytoparse++;
return c ? c : '\n';
}

void mcunput (int c)
{
yytoparse--;
/*assert (*yytoparse == c || (*yytoparse=='\0' && c == '\n'));*/
}

void yyerror (void)
{
errpos = gettoparse(yytoparse);
yyerrorflg = 1;
}
#endif
