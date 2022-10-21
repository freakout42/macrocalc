/* $Id: mctime.c,v 1.8 2016/10/31 06:35:14 axel Exp $
 */

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <arx_def.h>
#include "mc.h"
#include "mctime.h"

#define HOURSECONDS	(60*60)
#define DAYSECONDS	(24.*60.*60.)
#define unix2mc(ut)	((double)ut / DAYSECONDS + EPOCH)

static time_t mc2unix (double mt)
{
if (mt >= EPOCH && mt < ((double)LONG_MAX / DAYSECONDS + EPOCH))
	return (time_t)((mt - EPOCH) * DAYSECONDS);
else
	return (time_t)0;
}

void mclocal (double mct, struct tm **st)
{
time_t t;

#ifdef DEBUG
fprintf(stderr, "mclocal: %f->", mct);
#endif
t = mc2unix(mct);
#ifdef DEBUG
fprintf(stderr, "%ld\n", t);
#endif
*st = gmtime(&t);
}

static void localmc (struct tm *st, double *mct)
{
time_t t;

t = lib_mktm(st);
*mct = unix2mc(t);
}

double today (void)
{
time_t now;
struct tm *st;

time(&now);
st = localtime(&now);
now = lib_mktm(st);
return unix2mc(now);
}

double date (double year, double mon, double day)
{
struct tm st;
double mct;

#ifdef DEBUG
fprintf(stderr, "date: y=%f m=%f d=%f\n", year, mon, day);
#endif
if (year >= 0. && year < ROLLYEAR) year += 100.;
else if (year >= 1900.) year -= 1900.;
st.tm_year = year;
st.tm_mon = mon-1;
st.tm_mday = day;
st.tm_hour = st.tm_min = st.tm_sec = 0;
localmc(&st, &mct);
return mct;
}

double mctime (double hour, double min, double sec)
{
double mct;

mct = (sec+.01)/60.;
mct += min;
mct /= 60.;
mct += hour;
mct /= 24.;
return mct;
}

double year (double mct)
{
struct tm *st;

mclocal(mct, &st);
return st->tm_year+1900;
}

double month (double mct)
{
struct tm *st;

mclocal(mct, &st);
return st->tm_mon+1;
}

double day (double mct)
{
struct tm *st;

mclocal(mct, &st);
return st->tm_mday;
}

double hour (double mct)
{
struct tm *st;

mclocal(mct, &st);
return st->tm_hour;
}

double minute (double mct)
{
struct tm *st;

mclocal(mct, &st);
return st->tm_min;
}

double second (double mct)
{
struct tm *st;

mclocal(mct, &st);
return st->tm_sec;
}
