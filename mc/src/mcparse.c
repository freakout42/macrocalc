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
#ifdef WIERD
double yyvalue;
int yytype;
char *yyunit;
#else
CELLPTR pc;
#endif
char *yybuf;
char *yysparse; /* pointer to source formula */
char *yybparse; /* pointer to parsed formula */
char *yytoparse; /* pointer to left formular */
int yyerrorflg; /* error flag for lex-parser */
#endif

/* static char instr; flag for "in-string" */

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

#ifdef LOTUS
int parse2 (char *s, short *formula)
/* Parses the string s - returns the formula of the evaluated string
 */
#else
double parse (char *s, int *t, char *unit, char *parsed)
/* Parses the string s - returns the value of the evaluated string
 * and puts type unit and parsed formula
 */
#endif

{
cellv yycellv;

#ifdef LOTUS
char *polform;
short len;
#else
char yybuffer[MAXPARSED+1];
#endif
char yyinput[MAXPARSED+1];

yysparse = s;
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

#ifdef WIERD
yyunit = NULL;
yytype = CONSTANT;
#else
pc->val = &yycellv;
lcpunit(pc) = NULL;
cptyp(pc) = CONSTANT;
#endif
errno = 0;
#ifdef YYDEBUG
yydebug = 1;
#endif

yybegin();
yyparse();

yyclrbuf();

#ifdef LOTUS
len = yybuf - polform;
memcpy(formula, &len, sizeof(short));
#ifdef DEBUG
fprintf(stderr, "parse:\"%s\"=%d len=%d\n", s, cptyp(pc), len);
#endif
return cptyp(pc);
#else
#ifdef DEBUG
fprintf(stderr, "parse:\"%s\" -> %f type: %d error:%d\n",
                yybparse, cpvalue(pc), cptyp(pc), errno);
perror("parse");
#endif
if (t != NULL) *t = cptyp(pc);
if (unit != NULL && cpunit(pc) != NULL)
	{
	if (cptyp(pc)!=STRING && strspn(cpunit(pc), " ")==strlen(cpunit(pc))) *unit = '\0'; else strcpy(unit, cpunit(pc));
	}
return errno ? HUGE_VAL : cpvalue(pc);
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
/*switch (c)
 *{
 *case '\"':	instr = !instr; break;
 *case '{':	instr = TRUE; break;
 *case '}':	instr = FALSE; break;
 *}
 *if (!instr && islower(c)) c = toupper(c);
 */
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
