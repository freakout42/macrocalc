/* mcpary.y */

%{
int yylex();
void yyerror(const char *s);
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcellact.h"
#include "mcunit.h"
#include "mcfunc.h"
#ifdef	LOTUS
#include "lotus.h"
#include "mconvert.h"
#endif
#undef	stdout
#define stdout	stderr

#ifdef	LOTUS
static char	*yyopcode();
#else
static char	*yystr();
#endif
extern double	yyvalue;
extern int	yytype;
extern char	*yyunit;
extern char	*yybuf;

%}

%union	{
	struct CELLVALUE	value;
	struct CELLADR		cell;
	struct CELLADR		range[2];
	struct
	  {
	  unsigned char	oc;
	  double	(*f)();
	  }			func;
	char			*string;
}

%token	<value>		CON
%token	<string>	STR UNIT RANGEB JUST
%token	<cell>		CELL
%token	<range>		RANGE
%token	<func>		FUNC0 FUNC1 FUNC2 FUNC3 FUNC4 FUNCR FUNCS FUNCSC FUNCB
%token			AND OR XOR
%token			EQUAL NEQUAL GREATER LETHER GREATERE LETHERE EMARK
%token			PLUS MINUS TIMES DIVIDE EXP
%token			OPAREN CPAREN KOMMA PERIOD SEMI COLON
%token			UNITS UNITE
%token			STRCAT
%token			EOFCELL
%token			INCMD
%token			OUTCMD
%token			CMD
%token			BAD

%type	<value>		o e l
%type	<cell>		c
%type	<range>		r
%type	<string>	s u y

%left			UEXPR
%left			OR
%left			XOR
%left			AND
%left			EQUAL NEQUAL
%left			GREATER LETHER GREATERE LETHERE
%left			PLUS MINUS
%left			TIMES DIVIDE
%right			EXP
%left			UNIT
%right			UMINUS
%left			STRCAT

%%
o : l
	{
#ifdef	LOTUS
	*yybuf++ = F_RETURN;
#else
	yyvalue = $1.value;
	yyunit = $1.unit;
	yytype = CONSTANT;
#endif
	}
  | e
	{
#ifdef	LOTUS
	*yybuf++ = F_RETURN;
#else
	yyvalue = $1.value;
	yyunit = $1.unit;
#ifdef DEBUG
	fprintf (stderr, "mcpary: o value=\"%f %s\"\n", $1.value, $1.unit);
#endif
#endif
	}
  | e u %prec UEXPR
	{
#ifdef	LOTUS
	yyopcode (F_UNIT, $2, strlen($2)+1);
	*yybuf++ = F_RETURN;
#else
	yyvalue = unitconv ($1.value, $1.unit, $2); yyunit = $2;
#endif
	}
  | s
	{
#ifdef	LOTUS
	*yybuf++ = F_RETURN;
#else
	yyvalue = .0;
	yyunit = $1;
	yytype = STRING;
#endif
	}
  | INCMD CMD
	{
#ifndef	LOTUS
	yyvalue = .0;
#endif
	yytype = INCOMMAND;
	}
  | OUTCMD CMD
	{
#ifndef	LOTUS
	yyvalue = .0;
#endif
	yytype = OUTCOMMAND;
	}
  | EOFCELL
	{
#ifndef	LOTUS
	yyvalue = .0;
#endif
	yytype = EOFPIPE;
	}
  | BAD CMD
	{
#ifndef	LOTUS
	yyvalue = .0;
#endif
	yytype = TEXT;
	}
  | JUST CMD
	{
#ifndef	LOTUS
	yyvalue = .0;
#endif
	yytype = TEXT;
	}
  | RANGEB
	{
#ifndef	LOTUS
	yyvalue = .0;
#endif
	yytype = TEXT;
	}
  | error
	{
#ifndef	LOTUS
	yyvalue = .0;
#endif
	yytype = SYNERROR;
	}
  ;
e : e OR e
	{
#ifdef	LOTUS
	*yybuf++ = F_OR;
#else
	$$.value = $1.value || $3.value; $$.unit = NULL;
#endif
	}
  | e AND e
	{
#ifdef	LOTUS
	*yybuf++ = F_AND;
#else
	$$.value = $1.value && $3.value; $$.unit = NULL;
#endif
	}
  | e EQUAL e
	{
#ifdef	LOTUS
	*yybuf++ = F_EQUALS;
#else
	$$.value = $1.value == unitconv($3.value,$3.unit,$1.unit);
	$$.unit = NULL;
#endif
	}
  | e NEQUAL e
	{
#ifdef	LOTUS
	*yybuf++ = F_NOTEQUALS;
#else
	$$.value = $1.value != unitconv($3.value,$3.unit,$1.unit);
	$$.unit = NULL;
#endif
	}
  | e GREATER e
	{
#ifdef	LOTUS
	*yybuf++ = F_GREATER;
#else
	$$.value = $1.value > unitconv($3.value,$3.unit,$1.unit);
	$$.unit = NULL;
#endif
	}
  | e GREATERE e
	{
#ifdef	LOTUS
	*yybuf++ = F_GREATEREQUAL;
#else
	$$.value = $1.value >= unitconv($3.value,$3.unit,$1.unit);
	$$.unit = NULL;
#endif
	}
  | e LETHER e
	{
#ifdef	LOTUS
	*yybuf++ = F_LESS;
#else
	$$.value = $1.value < unitconv($3.value,$3.unit,$1.unit);
	$$.unit = NULL;
#endif
	}
  | e LETHERE e
	{
#ifdef	LOTUS
	*yybuf++ = F_LESSEQUAL;
#else
	$$.value = $1.value <= unitconv($3.value,$3.unit,$1.unit);
	$$.unit = NULL;
#endif
	}
  | e PLUS e
	{
#ifdef	LOTUS
	*yybuf++ = F_ADDITION;
#else
#ifdef DEBUG
	fprintf (stderr, "mcpary: plus\n");
#endif
	$$.value = $1.value + unitconv($3.value,$3.unit,$1.unit);
	$$.unit = $1.unit;
#ifdef DEBUG
	fprintf (stderr, "mcpary: plus value=\"%f %s\"\n", $$.value, $$.unit);
#endif
#endif
	}
  | e MINUS e
	{
#ifdef	LOTUS
	*yybuf++ = F_SUBSTRACTION;
#else
	$$.value = $1.value - unitconv($3.value,$3.unit,$1.unit);
	$$.unit = $1.unit;
#endif
	}
  | e TIMES e
	{
#ifdef	LOTUS
	*yybuf++ = F_MULTIPLY;
#else
	$$.value = $1.value * $3.value;
#ifdef DEBUG
	fprintf (stderr, "mcpary: TIMES value=\"%f\"\n", $$.value);
#endif
	$$.unit = yybuf;
	yybuf = unitmult (yybuf, $1.unit, $3.unit);
#ifdef DEBUG
	fprintf (stderr, "mcpary: TIMES unit=\"%s\"\n", $$.unit);
#endif
#endif
	}
  | e DIVIDE e
	{
#ifdef	LOTUS
	*yybuf++ = F_DIVISION;
#else
#ifdef DEBUG
	fprintf (stderr, "mcpary: DIVIDE errno=%d\n", errno);
#endif
	if ($3.value==0.)
		{
		errno = ERANGE;
		$$.value = HUGE_VAL;
		}
	else	$$.value = $1.value / $3.value;
	$$.unit = yybuf;
	yybuf = unitdiv (yybuf, $1.unit, $3.unit);
#ifdef DEBUG
	fprintf (stderr, "mcpary: DIVIDE value=\"%f/%f=%f %s\" errno=%d\n",
			$1.value, $3.value, $$.value, $$.unit, errno);
#endif
#endif
	}
  | e EXP e
	{
#ifdef	LOTUS
	*yybuf++ = F_EXPONENT;
#else
	$$.value = pow ($1.value, $3.value);
	$$.unit = NULL;
#endif
	}
  | MINUS e %prec UMINUS
	{
#ifdef	LOTUS
	*yybuf++ = F_UNARYMINUS;
#else
	$$.value = - ($2.value);
	$$.unit = $2.unit;
#endif
	}
  | FUNC0 CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = $1.oc;
#else
	$$.value = (*$1.f) ();
	$$.unit = NULL;
#endif
	}
  | FUNC1 e CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = $1.oc;
#else
	$$.value = (*$1.f) ($2.value);
	$$.unit = $2.unit;
#endif
	}
  | FUNC2 e KOMMA e CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = $1.oc;
#else
	$$.value = (*$1.f) ($2.value, $4.value);
	$$.unit = NULL;
#endif
	}
  | FUNC3 e KOMMA e KOMMA e CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = $1.oc;
#else
	$$.value = (*$1.f) ($2.value, $4.value, $6.value);
	$$.unit = NULL;
#endif
	}
  | FUNC4 e KOMMA e KOMMA e KOMMA e CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = $1.oc;
#else
	$$.value = (*$1.f) ($2.value, $4.value, $6.value, $8.value);
	$$.unit = NULL;
#endif
	}
  | FUNCR r CPAREN
	{
#ifdef	LOTUS
#ifdef DEBUG
	fprintf (stderr, "mcpary: FUNCR opcode=%d\n", $1.oc);
#endif
	*yybuf++ = $1.oc;
	*yybuf++ = 1;
#else
	$$.value = (*$1.f) ($2[0], $2[1]);
	$$.unit = yyunit;
#endif
	}
  | FUNCS s CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = $1.oc;
#else
	$$.value = (*$1.f) ($2);
	$$.unit = NULL;
#endif
	}
  | l
  | OPAREN e CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = F_PARANTHESES;
#else
	$$ = $2;
#endif
	}
  | CON
	{
#ifdef	LOTUS
	$$.value = lib_2iee($1.value);
	yyopcode (F_CONSTANT, &$$.value, sizeof(double));
#else
	$$.value = $1.value;
	$$.unit = NULL;
#ifdef DEBUG
	fprintf (stderr, "mcpary: CON value=\"%f\"\n", $$.value);
#endif
#endif
	}
  | CON u %prec UNIT
	{
#ifdef	LOTUS
	lib_cano($1.value);
	$$.value = lib_2iee($1.value);
	yyopcode (F_CONSTANT, &$$.value, sizeof(double));
	$$.unit = yyopcode (F_UNIT, $2, strlen($2)+1);
#else
	$$.value = $1.value;
	$$.unit = $2;
#ifdef DEBUG
	fprintf (stderr, "mcpary: CON u1 value=\"%f\"\n", $$.value);
#endif
	unitconv ($1.value,$2,$2);
#ifdef DEBUG
	fprintf (stderr, "mcpary: con u2 value=\"%f\"\n", $$.value);
#endif
#endif
	}
  | c
	{
#ifdef	LOTUS
	CELLADR123 c123;

	convertcelladr (&c123, &$1);
#ifdef DEBUG
	fprintf (stderr, "mcpary: c123 c=%d r=%d\n", c123.col, c123.row);
#endif
	yyopcode (F_VARIABLE, &c123, sizeof(c123));
#else
	CELLPTR	cp;

	if ((cp = cell (adrval($1.col), adrval($1.row))))
		{
#ifdef DEBUG
		fprintf (stderr, "mcpary: c cp=%08x cpv=%08x\n", cp, cpv(cp));
#endif
	 	$$.value = cpvalue (cp);
		$$.unit  = cpunit (cp);
		}
	else
		{
		$$.value = 0.;
		$$.unit  = NULL;
		}
#endif
	}
  ;
l : FUNCSC s KOMMA c CPAREN
	{
#ifdef	LOTUS
	*yybuf++ = $1.oc;
#else
	$$.value = (*$1.f) ($2, adrval($4.col), adrval($4.row));
	$$.unit = NULL;
#endif
	}
  ;
s : s s
	{
#ifdef	LOTUS
#ifdef DEBUG
	fprintf (stderr, "mcpary: F_STRINGCAT opcode=%d\n", F_STRINGCAT);
#endif
	*yybuf++ = F_STRINGCAT;
#else
	$$ = yystr ($1); yybuf--;
	yystr ($2);
#endif
	}
  | STR
	{
#ifdef	LOTUS
	yyopcode (F_STRING, $1, strlen($1)+1);
#else
	$$ = yystr ($1);
#endif
	}
  | STRCAT c
	{
#ifdef	LOTUS
	CELLADR123 c123;

	convertcelladr (&c123, &$2);
	yyopcode (F_VARIABLE, &c123, sizeof(c123));
#else
	$$ = celltext (adrval($2.col), adrval($2.row));
#ifdef DEBUG
	fprintf (stderr, "mcpary: STRCAT s=\"%s\"\n", $$);
#endif
#endif
	}
  ;
r : c PERIOD c
	{
#ifdef	LOTUS
	CELLADR123 c123[2];

	convertcelladr (&c123[0], &$1);
	convertcelladr (&c123[1], &$3);
	yyopcode (F_RANGE, &c123, sizeof(c123));
#endif
	$$[0].col = adrval($1.col);
	$$[0].row = adrval($1.row);
	$$[1].col = adrval($3.col);
	$$[1].row = adrval($3.row);
	yytype = FORMULA;
	}
  | RANGE
	{
#ifdef	LOTUS
	CELLADR123 c123[2];

	convertcelladr (&c123[0], &$1[0]);
	convertcelladr (&c123[1], &$1[1]);
	yyopcode (F_RANGE, &c123, sizeof(c123));
#endif
	memcpy ($$, $1, sizeof(struct CELLADR)*2);
	yytype = FORMULA;
	}
  | RANGEB
	{
#ifdef	LOTUS
	CELLADR123 c123[2];
#endif
	errno = UNRECRANGE;
	$$[0].col = 0;
	$$[0].row = 0;
	$$[1].col = 0;
	$$[1].row = 0;
#ifdef	LOTUS
	convertcelladr (&c123[0], &$$[0]);
	convertcelladr (&c123[1], &$$[1]);
	yyopcode (F_RANGE, &c123, sizeof(c123));
#endif
	yytype = FORMULA;
	}
  ;
c : CELL
	{
#ifdef DEBUG
	fprintf (stderr, "mcpary: cell col=%d row=%d\n", $1.col, $1.row);
#endif
	$$ = $1;
	yytype = FORMULA;
	}
  | r
	{
	$$ = $1[0];
	}
  ;
u : UNITS y UNITE
	{
	$$ = $2;
	}
  ;
y : UNIT
	{
#ifdef	LOTUS
	$$ = strcpy (yyunit, $1);
#else
	$$ = yybuf;
	yybuf = unitnorm (yybuf, $1);
#endif
	}
  | y UNIT
	{
#ifdef	LOTUS
	$$ = strcat (yyunit, $2);
#else
	$$ = yybuf;
	yybuf = unitmult (yybuf, $1, $2);
#endif
	}
  ;
%%

#ifdef	LOTUS
static char	*yyopcode (opcode, buffer, length)
	unsigned char 	opcode;
	char		*buffer;
	size_t		length;
{
char	*bufp;

#ifdef DEBUG
fprintf (stderr, "mcpary: lotus opcode=\"%d\"\n", opcode);
#endif
*yybuf++ = opcode;
memcpy (yybuf, buffer, length);
bufp	=  yybuf;
yybuf	+= length;
return bufp;
}
#else

static char *yystr (s)	char *s;
{
char	*d;

d	=  yybuf;
strcpy (d, s);
yybuf	+= strlen (d) + 1;
return d;
}
#endif
