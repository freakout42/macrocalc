/* $Id: lib_mktm.c,v 1.2 1999/10/14 18:22:30 axel Exp $
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define	YEARDAYS	365l
#define	DAYHOURS	24l
#define	HOURMINUTES	60l
#define	MINUTESECONDS	60l
#define	HOURSECONDS	(HOURMINUTES*MINUTESECONDS)
#define	DAYSECONDS	(DAYHOURS*HOURSECONDS)
#define	EPOCHDAY	25567l
#define	EPOCH		(EPOCHDAY*DAYSECONDS)
enum month {
	JANUARY=0, FEBRUARY, MARCH,
	APRIL, MAY, JUNE,
	JULY, AUGUST, SEPTEMBER,
	OCTOBER, NOVEMBER, DECEMBER
	};

time_t lib_mktm (struct tm *tp)

{
time_t		t;
static int	months[] =
		{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

t =  tp->tm_year * YEARDAYS;
t += (tp->tm_year-1) / 4;
t += months[tp->tm_mon];
t +=    tp->tm_year % 4 == 0
     && ( tp->tm_year % 100 != 0 || (tp->tm_year+300) % 400 == 0 )
     && tp->tm_mon > FEBRUARY ? 1 : 0;
t += tp->tm_mday-1;
t -= (time_t)EPOCHDAY;
t *= DAYSECONDS;
t += tp->tm_hour * HOURSECONDS;
t += tp->tm_min * MINUTESECONDS;
t += tp->tm_sec;
return t;
}
