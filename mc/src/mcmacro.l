%{
/* $Id: mcmacro.l,v 1.4 1997/04/11 13:25:35 axel Exp $
 */

#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include "arx_def.h"
#include "str_def.h"
#include "cur_def.h"

struct Macrotab
	{
	char		*name;
	int		key;
	} macrotab[]	=
	{{"DOWN", KEY_DOWN}
	,{"LEFT", KEY_LEFT}
	,{"RIGHT", KEY_RIGHT}
	,{"UP", KEY_UP}
	,{"D", KEY_DOWN}
	,{"L", KEY_LEFT}
	,{"R", KEY_RIGHT}
	,{"U", KEY_UP}
	,{"PGDN", KEY_NPAGE}
	,{"PGUP", KEY_PPAGE}
	,{"BIGRIGHT", KEY_TAB}
	,{"BIGLEFT", KEY_BTAB}
	,{"RETURN", KEY_RETURN}
	,{"MENU", '/'}
	,{"MARK", '.'}
	,{"DEL", KEY_DC}
	,{"DELETE", KEY_DC}
	,{"EDIT", KEY_F(2)}
	,{"END", KEY_LL}
	,{"ESC", KEY_ESC}
	,{"ESCAPE", KEY_ESC}
	,{"GOTO", KEY_F(5)}
	,{"HELP", KEY_F(1)}
	,{"HOME", KEY_HOME}
	,{"INS", KEY_IC}
	,{"INSERT", KEY_IC}
	,{"CALC", KEY_F(9)}
	,{"BLANK", ' '}
	,{NULL, 0}
	};

void putout (s)
	char *s;
{
for (; *s; s++) printf ("%d\n", *s);
}
%}
n	[0-9]
a	[A-Za-z]
an	[0-9A-Za-z]
w	[ \t\n]+
c	"#".*
o	[\t\n]+
m	"{"{a}+{w}*{n}*"}"
v	"$"{an}+
%%
~	printf ("%d\n", '\n');
{v}	{
	char	*ev;
	
	if ((ev = getenv(yytext+1)) != NULL)
		putout (ev);
	else	fprintf (stderr, "mcmacro: environment variable \"%s\" not found!\n",
			yytext);
	}
{m}	{
	struct Macrotab	*m;
	int		i;

	str_upr (yytext);
	for (m = macrotab; m->name; m++)
		{
		if (strncmp (m->name, yytext+1, strlen(m->name)) == 0) break;
		}
	if (m->key)
		{
		i = atoi (yytext+1+strlen(m->name));
		for (i = i ? i : 1; i; i--)
			printf ("%d\n", m->key);
		}
	else	fprintf (stderr, "mcmacro: unrecognized macroname \"%s\"!\n",
			yytext);
	}
^{c}	;
{o}	;
.	printf ("%d\n", yytext[0]);
%%
