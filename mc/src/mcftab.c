/* mcftab.c 1.2 1997/04/04 06:38:34 axel */

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
	 ftabentry ("ABS",	FUNC1,	L3_ABS,	fabs)
	,ftabentry ("ACOS",	FUNC1,	L3_ACOS,	acos)
	,ftabentry ("ASIN",	FUNC1,	L3_ASIN,	asin)
	,ftabentry ("ATAN",	FUNC1,	L3_ATAN,	atan)
	,ftabentry ("COS",	FUNC1,	L3_COS,	cos)
	,ftabentry ("COSH",	FUNC1,	L3_COSH,	cosh)
	,ftabentry ("EXP",	FUNC1,	L3_EXP,	exp)
	,ftabentry ("INT",	FUNC1,	L3_INT,	floor)
	,ftabentry ("LN",	FUNC1,	L3_LN,	log)
	,ftabentry ("LOG",	FUNC1,	L3_LOG,	log10)
	,ftabentry ("SIN",	FUNC1,	L3_SIN,	sin)
	,ftabentry ("SINH",	FUNC1,	L3_SINH,	sinh)
	,ftabentry ("SQRT",	FUNC1,	L3_SQRT,	sqrt)
	,ftabentry ("TAN",	FUNC1,	L3_TAN,	tan)
	,ftabentry ("TANH",	FUNC1,	L3_TANH,	tanh)
	,ftabentry ("AVG",	FUNCR,	L3_AVG,	avg)
	,ftabentry ("COUNT",	FUNCR,	L3_COUNT,	count)
	,ftabentry ("MAX",	FUNCR,	L3_MAX,	maximum)
	,ftabentry ("MIN",	FUNCR,	L3_MIN,	minimum)
	,ftabentry ("STD",	FUNCR,	L3_STD,	std)
	,ftabentry ("SUM",	FUNCR,	L3_SUM,	sum)
	,ftabentry ("VAR",	FUNCR,	L3_VAR,	var)
	,ftabentry ("CTERM",	FUNC3,	L3_CTERM,	cterm)
	,ftabentry ("DDB",	FUNC4,	L3_DDB,	ddb)
	,ftabentry ("FV",	FUNC3,	L3_FV,	fv)
	,ftabentry ("PMT",	FUNC3,	L3_PMT,	pmt)
	,ftabentry ("PV",	FUNC3,	L3_PV,	pv)
	,ftabentry ("RATE",	FUNC3,	L3_RATE,	rate)
	,ftabentry ("SLN",	FUNC3,	L3_SLN,	sln)
	,ftabentry ("SYD",	FUNC4,	L3_SYD,	syd)
	,ftabentry ("TERM",	FUNC3,	L3_TERM,	term)
	,ftabentry ("DATE",	FUNC3,	L3_DATE,	date)
	,ftabentry ("TIME",	FUNC3,	L3_TIME,	mctime)
	,ftabentry ("TODAY",	FUNC0,	L3_TODAY,	today)
	,ftabentry ("YEAR",	FUNC1,	L3_YEAR,	year)
	,ftabentry ("MONTH",	FUNC1,	L3_MONTH,	month)
	,ftabentry ("DAY",	FUNC1,	L3_DAY,	day)
	,ftabentry ("HOUR",	FUNC1,	L3_HOUR,	hour)
	,ftabentry ("MINUTE",	FUNC1,	L3_MINUTE,	minute)
	,ftabentry ("SECOND",	FUNC1,	L3_SECOND,	second)
	,ftabentry ("LENGTH",	FUNCS,	L3_LENGTH,	length)
	,ftabentry ("IF",	FUNC3,	L3_IF,	mcif)
	,ftabentry ("LINK",	FUNCSC,	L3_LINK,	mclink)
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
