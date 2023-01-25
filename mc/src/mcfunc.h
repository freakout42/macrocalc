/* mcfunc.h 1.1 1995/02/19 18:47:21 axel */

#define L3_ABS 33
#define L3_INT 34
#define L3_SQRT 35
#define L3_LOG 36
#define L3_LN 37
#define L3_SIN 39
#define L3_COS 40
#define L3_TAN 41
#define L3_COSH 42 /* not offical */
#define L3_ATAN 43
#define L3_ASIN 44
#define L3_ACOS 45
#define L3_EXP 46
#define L3_SINH 47 /* not offical */
#define L3_TANH 48  /* not offical */
#define L3_LINK 49 /* not offical */
#define L3_DATE 54
#define L3_PMT 56
#define L3_TODAY 55
#define L3_PV 57
#define L3_FV 58
#define L3_IF 59
#define L3_DAY 60
#define L3_MONTH 61
#define L3_YEAR 62
#define L3_TIME 64
#define L3_HOUR 65
#define L3_MINUTE 66
#define L3_SECOND 67
#define L3_LENGTH 70
#define L3_SUM 80
#define L3_AVG 81
#define L3_COUNT 82
#define L3_MIN 83
#define L3_MAX 84
#define L3_VAR 87
#define L3_STD 88
#define L3_RATE 116
#define L3_TERM 117
#define L3_CTERM 118
#define L3_SLN 119
#define L3_SYD 120
#define L3_DDB 121

double	cterm ();
double	ddb ();
double	fv ();
double	pmt ();
double	pv ();
double	rate ();
double	sln ();
double	syd ();
double	term ();

double	avg ();
double	count ();
double	maximum ();
double	minimum ();
double	std ();
double	sum ();
double	var ();

double	date ();
double	mctime ();
double	today ();
double	year ();
double	month ();
double	day ();
double	hour ();
double	minute ();
double	second ();

double	length ();
double	mcif ();
double	mclink ();

/* user function example
 * double	mcuser ();
 */
