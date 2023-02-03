/* pol2tex.c 1.10 2001/11/07 12:23:51 axel
 * reverse polish formula code to text
 */

#define	LOTUS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>
#include "lotus.h"
#include "mcelldef.h"
#include "mcelladr.h"
#include "mcfunc.h"
#include "mcftab.h"
#include "mcell123.h"
#include "f.tab.h"

int pol2tex (char *tex, unsigned char *code)
{
short opcode;
int sp;
char *stack[100];
short i;
double d;
char s[256];
CELLADR123 a;
struct Functab *f;
char *n;
int pn;
union	{
	unsigned char		*c;
	short			*i;
	double			*d;
	CELLADR123		*a;
	} p;
#define moveitem(typ)	memcpy (&typ, p.typ++, sizeof(typ))

p.c		= code;
sp		= 0;
while ((opcode = *p.c++) != F_RETURN)
	{
#ifdef DEBUG
	fprintf (stderr, "pol2tex: p=%d\n", p);
	fprintf (stderr, "pol2tex: opcode=%d\n", opcode);
#endif
	switch (opcode)
	 {
	 case F_CONSTANT:
	 	moveitem (d);
	 	d = lib_iee2 (d);
		sprintf (tex, "%f", d);
#ifdef DEBUG
		fprintf (stderr, "pol2tex: constant=%f\n", d);
#endif
		goto shift;
	 case F_VARIABLE:
	 	moveitem (a);
		lib_cano(a.col);
		lib_cano(a.row);
		celladrs123 (&a, tex);
#ifdef DEBUG
		fprintf (stderr, "pol2tex: variable c=%d r=%d\n", a.col, a.row);
#endif
		goto shift;
	 case F_RANGE:
		moveitem (a);
		lib_cano(a.col);
		lib_cano(a.row);
		n = celladrs123 (&a, tex);
		strcat (tex, "..");
		n += 2;
		moveitem (a);
		lib_cano(a.col);
		lib_cano(a.row);
		celladrs123 (&a, n);
		goto shift;
	 case F_RETURN:
		break;
	 case F_PARANTHESES:
		sprintf (tex, "(%s)", stack[sp-1]);
		goto change;
	 case F_INTEGER:
		moveitem (i);
		lib_cano(i);
		sprintf (tex, "%d", i);
		goto shift;
	 case F_STRING:
	 	sprintf (tex, "\"%s\"", p.c);
		p.c += strlen((char*)p.c) + 1;
		goto shift;
	 case F_UNARYMINUS:
		sprintf (tex, "-%s", stack[sp-1]);
		goto change;
	 case F_ADDITION:
		strcpy (s, "+");
		goto reducepair;
	 case F_SUBSTRACTION:
		strcpy (s, "-");
		goto reducepair;
	 case F_MULTIPLY:
		strcpy (s, "*");
		goto reducepair;
	 case F_DIVISION:
		strcpy (s, "/");
		goto reducepair;
	 case F_EXPONENT:
		strcpy (s, "^");
		goto reducepair;
	 case F_EQUALS:
		strcpy (s, "=");
		goto reducepair;
	 case F_NOTEQUALS:
		strcpy (s, "<>");
		goto reducepair;
	 case F_LESSEQUAL:
		strcpy (s, "<=");
		goto reducepair;
	 case F_GREATEREQUAL:
		strcpy (s, ">=");
		goto reducepair;
	 case F_LESS:
		strcpy (s, "<");
		goto reducepair;
	 case F_GREATER:
		strcpy (s, ">");
		goto reducepair;
	 case F_AND:
		strcpy (s, "#AND#");
		goto reducepair;
	 case F_OR:
		strcpy (s, "#OR#");
		goto reducepair;
	 case F_NOT:
		sprintf (tex, "#NOT#%s", stack[sp-1]);
		goto change;
	 case F_UNARYPLUS:
		break;
	 case F_STRINGCAT:
		strcpy (s, " ");
		goto reducestrpair;
	 default:
		for (f = functab; f->name && f->opcode!=opcode; f++);
		if (f->opcode != 0)
			{
			switch (f->token)
			 {
			 case FUNC0:
				sprintf (tex, "@%s()", f->name);
				goto shift;
			 case FUNCR:
				pn = *p.c++;
				while (--pn)
					{
					free (stack[--sp]);
					fprintf (stderr, "mc2wks: "
					"multiple arguments not supported\n");
					}
				/*FALLTHRU*/
			 case FUNC1:
			 case FUNCS:
				sprintf (tex, "@%s(%s)", f->name, stack[sp-1]);
				goto change;
			 case FUNC2:
				sprintf (tex, "@%s(%s%c%s)", f->name, stack[sp-2], SEPCH, stack[sp-1]);
				goto reduce;
			 case FUNC3:
				sprintf (tex, "@%s(%s%c%s%c%s)", f->name, stack[sp-3], SEPCH, stack[sp-2], SEPCH, stack[sp-1]);
				goto reduce2;
			 case FUNC4:
				sprintf (tex, "@%s(%s%c%s%c%s%c%s)", f->name, stack[sp-4], SEPCH, stack[sp-3], SEPCH, stack[sp-2], SEPCH, stack[sp-1]);
				goto reduce3;
			 }
			}
		else
			{
			fprintf (stderr,
			   "mc2wks: unknown opcode %d in formula.\n", opcode);
			return (RET_ERROR);
			}
		break;
	 shift:
		stack[sp++] = strdup(tex);
		break;
	 reducepair:
		sprintf (tex, "%s%s%s", stack[sp-2], s, stack[sp-1]);
		goto reduce;
	 reducestrpair:
		sprintf (tex, "%c%s%s%c%s",
			*stack[sp-2]=='"'?' ':'#', stack[sp-2], s,
			*stack[sp-1]=='"'?' ':'#', stack[sp-1]);
		goto reduce;
	 reduce3:
	 	free (stack[--sp]);
	 reduce2:
	 	free (stack[--sp]);
	 reduce:
	 	free (stack[--sp]);
	 change:
	 	free (stack[sp-1]);
		stack[sp-1]	= strdup(tex);
		break;
	 }
	}
return (RET_SUCCESS);
}
