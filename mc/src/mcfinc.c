/* $Id: mcfinc.c,v 1.2 1995/02/24 10:22:52 axel Rel $
 */

#include <math.h>
#include <errno.h>
#include <arx_def.h>

double cterm (double interest, double futurevalue, double presentvalue)
{
return log (futurevalue/presentvalue) / log (1+interest);
}

double ddb (double cost, double salvage, double life, double period)
{
int	i, n = (int) (period+.5);
double	d, val = cost;

if (n < 1 || n > (int)(life+.5))
	{
	errno = ERANGE;
	return HUGE_VAL;
	}
for (i = 1; i <= n; i++)
	{
	d	= (val*2)/life;
	val	-= d;
	}
val += d;
return val > salvage ? min (d, val-salvage) : 0.;
}

double fv (double annuity, double interestrate, double period)
{
return annuity * (pow(interestrate+1.,period) - 1.) / interestrate;
}

double pmt (double principal, double interest, double period)
{
return principal * (interest / (1. - pow(interest+1.,-period)));
}

double pv (double annuity, double interestrate, double period)
{
return annuity * (1.-pow(1.+interestrate,-period)) / interestrate;
}

double rate (double futurevalue, double presentvalue, double n)
{
return pow (futurevalue / presentvalue, 1./n) - 1.;
}

double sln (double cost, double salvage, double life)
{
return (cost-salvage) / life;
}

double syd (double cost, double salvage, double life, double period)
{
return (cost-salvage) * (life-period+1.) / (life * (life+1.)/2.);
}

double term (double payment, double interest, double futurevalue)
{
return log (1.+(futurevalue*interest/payment)) / log (1.+interest);
}
