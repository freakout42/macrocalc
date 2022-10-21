/* $Id: mcftab.c,v 1.2 1997/04/04 06:38:34 axel Exp $
 */

#include <stddef.h>
#include <math.h>
#include "mcfunc.h"
#include "f.tab.h"

#define ftabentry(name,type,opcode,func)	{name, type, opcode, func}

/* for each function of mc a entry in this function table must exist.  */

struct Functab
	{

	char	*name;		/* name of the function used in cell formula
				 * must be in upper case
				 */

	int	token;		/* type of the function
				 * FUNC0 .. FUNC4 0 to 4 double parameters
				 * FUNCS one char* parameter
				 */

	unsigned char opcode;	/* opcode of lotus formula
				 * only required in mc2wks
				 */

	double	(*func)();	/* name of the corresponding C function
				 * must be a double returning function
				 */

	} functab[] =
	{
	 ftabentry ("ABS",	FUNC1,	33,	fabs)
	,ftabentry ("ACOS",	FUNC1,	45,	acos)
	,ftabentry ("ASIN",	FUNC1,	44,	asin)
	,ftabentry ("ATAN",	FUNC1,	43,	atan)
	,ftabentry ("COS",	FUNC1,	40,	cos)
	,ftabentry ("COSH",	FUNC1,	0,	cosh)
	,ftabentry ("EXP",	FUNC1,	46,	exp)
	,ftabentry ("INT",	FUNC1,	34,	floor)
	,ftabentry ("LN",	FUNC1,	37,	log)
	,ftabentry ("LOG",	FUNC1,	36,	log10)
	,ftabentry ("SIN",	FUNC1,	39,	sin)
	,ftabentry ("SINH",	FUNC1,	0,	sinh)
	,ftabentry ("SQRT",	FUNC1,	35,	sqrt)
	,ftabentry ("TAN",	FUNC1,	41,	tan)
	,ftabentry ("TANH",	FUNC1,	0,	tanh)
	,ftabentry ("AVG",	FUNCR,	81,	avg)
	,ftabentry ("COUNT",	FUNCR,	82,	count)
	,ftabentry ("MAX",	FUNCR,	84,	maximum)
	,ftabentry ("MIN",	FUNCR,	83,	minimum)
	,ftabentry ("STD",	FUNCR,	88,	std)
	,ftabentry ("SUM",	FUNCR,	80,	sum)
	,ftabentry ("VAR",	FUNCR,	87,	var)
	,ftabentry ("CTERM",	FUNC3,	118,	cterm)
	,ftabentry ("DDB",	FUNC4,	121,	ddb)
	,ftabentry ("FV",	FUNC3,	58,	fv)
	,ftabentry ("PMT",	FUNC3,	56,	pmt)
	,ftabentry ("PV",	FUNC3,	57,	pv)
	,ftabentry ("RATE",	FUNC3,	116,	rate)
	,ftabentry ("SLN",	FUNC3,	119,	sln)
	,ftabentry ("SYD",	FUNC4,	120,	syd)
	,ftabentry ("TERM",	FUNC3,	117,	term)
	,ftabentry ("DATE",	FUNC3,	54,	date)
	,ftabentry ("TIME",	FUNC3,	64,	mctime)
	,ftabentry ("TODAY",	FUNC0,	55,	today)
	,ftabentry ("YEAR",	FUNC1,	62,	year)
	,ftabentry ("MONTH",	FUNC1,	61,	month)
	,ftabentry ("DAY",	FUNC1,	60,	day)
	,ftabentry ("HOUR",	FUNC1,	65,	hour)
	,ftabentry ("MINUTE",	FUNC1,	66,	minute)
	,ftabentry ("SECOND",	FUNC1,	67,	second)
	,ftabentry ("LENGTH",	FUNCS,	70,	length)
	,ftabentry ("IF",	FUNC3,	59,	mcif)
	,ftabentry ("LINK",	FUNCSC,	0,	mclink)
/*
 * user defined fuctions start here
 * include one definition line for each added user-function
 * don't forget to add the function also in mcfunc.h
 * see mcuser.c for an example of a user-function.
 *
 *	,ftabentry ("USER",	FUNC1,	0,	mcuser)
 */

/* must be last entry, do not remove */
	,ftabentry (NULL,	FUNCB,	0,	NULL)
	};
