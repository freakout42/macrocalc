/* mcunit.c */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <arx_def.h>
#include <str_def.h>
#include "mc.h"
#include "mcunit.h"

#define MCUNITS
#ifdef MCUNITS
#define UNITPIPE
#endif

#define	HAVE	0
#define	WANT	1
#define	FIRST	2

static FILE *unitin = (FILE*)1;
static FILE *unitout = NULL;
char pcmd[MAXFILE+10];
static int units = FALSE;

int unitinit (void)
/* init unit conversion */
{
#ifdef UNITPIPE
sprintf (pcmd, "%s/bin/mcunits", libpath);
units = !lib_twpo (&unitin, &unitout, pcmd);
#else
units = TRUE;
#endif
return !units;
} /* unitinit */

int unitclose (void)
/* close unit server */
{
if (!units) return RET_SUCCESS;
#ifdef UNITPIPE
return fclose(unitin) | fclose(unitout);
#else
return RET_SUCCESS;
#endif
} /* unitclose */

static int unitread (char *inbuf)
{
char *s = inbuf;
int c;

if (fflush (unitout))
	{
#ifdef DEBUG
	fprintf (stderr, "unit: error flush!\n");
#endif
	return EOF;
	}
errno = 0;
#ifdef DEBUG
while (read (fileno(unitin), inbuf, 1) == 1)
#else
while ((c = fgetc (unitin)) != EOF)
#endif
	{
#ifdef DEBUG
	if (errno) {fprintf (stderr, "unit: error %d fgetc!\n", errno); errno=0;}
	inbuf++;
#else
	*inbuf++ = c;
#endif
	*inbuf = '\0';
	if (errno==ESPIPE) errno = 0;
#ifdef UNITPIPE
	if (strstr (s, "have: ")) return HAVE;
	if (strstr (s, "want: ")) return WANT;
#endif
	}
return *inbuf++ = EOF;
}

double unitconv (double s, char *su, char *tu)
/* convert from source unit to target unit */
{
static int	mode	= FIRST;
float		m, d;
char		inbuf[MAXINPUT+1]	= "";
char		*starp;

if (!units) return s;
#ifdef DEBUG
fprintf (stderr, "unitconv: %f [%s] = ? [%s]\n", s, su, tu);
/* unitconv: 1.000000 [(null)] = ? [ m] [Key]
   unitconv: 1.000000 [ m] = ? [ m /h] [Key]
You have:  m
You want:  m /h
Conformability
1 metre
1.50916e+30 metre-1 gramme-1 second
if ((su==NULL || !*su) && (tu==NULL || !*tu)) return s;
if (su==NULL || tu==NULL) {errno = CONFUNIT; return HUGE_VAL;}
 */
#endif
if (su==NULL || !*su) return s;
if (tu==NULL || !*tu) {errno = CONFUNIT; return HUGE_VAL;}

#ifdef UNITPIPE
if (mode == FIRST)
	{
	mode = unitread (inbuf);
	}
while (mode != HAVE)
	{
	fputc ('\n', unitout);
	mode = unitread (inbuf);
	}
if (mode==EOF) return HUGE_VAL;
#else
if (lib_twpo (&unitin, &unitout, "units")) return HUGE_VAL;
#endif
fprintf (unitout, "%s\n", su);
#ifdef DEBUG
fprintf (stderr, "unitconv: put mcunits: \"%s\"\n", su);
#endif
#ifdef UNITPIPE
if ((mode = unitread (inbuf)) != WANT) {errno = UNRECUNIT; return HUGE_VAL;}
#endif
fprintf (unitout, "%s\n", tu);
#ifndef UNITPIPE
fclose(unitout);
#endif
mode = unitread (inbuf);
#ifdef DEBUG
fprintf (stderr, "unitconv: get mcunits: \"%s\"\n", inbuf);
#endif
if (((starp = strchr(inbuf, '*'))==NULL) || sscanf(starp, "* %f / %f", &m, &d) != 2)
	{
	errno = strstr (inbuf, "onformability") ? CONFUNIT : UNRECUNIT;
	return HUGE_VAL;
	}
#ifdef DEBUG
fprintf (stderr, "unitconv: %f [%s] = %f [%s]\n\n",
	s, su, m>1. ? s * m : s / d, tu);
#endif
#ifndef UNITPIPE
fclose(unitin);
#endif
return m>1. ? s * m : s / d;
} /* unitconv */

char *unitnorm (char *n, char *u)
/* unit normalize */
{
#ifdef DEBUG
char *nn = n;
#endif
if (u==NULL) return n+1;
*n++	= ' ';
for (u=u; *u; u++) if (!isspace(*u)) *n++ = *u;
*n++	= '\0';
#ifdef DEBUG
fprintf (stderr, "unitnorm: \"%s\" <- \"%s\"\n", nn, u);
#endif
return n;
} /* unitnorm */

char *unitinv (char *n, char *u)
/* unit invert */
{
char	ub[MAXINPUT+1];
char	*unit;
#ifdef DEBUG
char *nn = n;
#endif

if (u==NULL) return n;
strcpy (ub, u);
*n = '\0';
for (unit = strtok (ub, " "); unit != NULL; unit = strtok (NULL, " "))
	{
	strcat (n, " ");
	if (unit[0] == '/') strcat (n, unit+1);
	else	{
		strcat (n, "/");
		strcat (n, unit);
		}
	}
#ifdef DEBUG
fprintf (stderr, "unitinv: \"%s\" <- \"%s\" errno:%d\n", n, u, errno);
#endif
errno = 0;	/* strtok produces "Invalid Argument" Linux */
return n + strlen(n) + 1;
} /* unitinv */

char *unitmult (char *n, char *u1, char *u2)
/* unit multiplication */
{
*n = '\0';
#ifdef DEBUG
fprintf (stderr, "unitmult: \"%s\" * \"%s\"\n", u1, u2);
fprintf (stderr, "unitmult: \"%lx\" <- \"%lx\" * \"%lx\"\n", n, u1, u2);
#endif
if (u1!=NULL) strcpy (n, u1);
return unitnorm (n + strlen (n), u2);
} /* unitmult */

char *unitdiv (char *n, char *u1, char *u2)
/* unit division */
{
*n = '\0';
if (u1!=NULL) strcpy (n, u1);
return unitinv (n+strlen(n), u2);
} /* unitdiv */

#ifdef TEST
int main (argc, argv)
	int argc;
	char *argv[];
{
double	sf, tf;
char	su[1024];
char	buf[2048];
char	*sp;

#ifdef DEBUG
fprintf (stderr, "mcunit: start\n");
#endif
if (unitinit())
	{
	fprintf (stderr, "unitinit failed\n");
	exit (EXIT_FAILURE);
	}
#ifdef DEBUG
fprintf (stderr, "mcunit: unitinit done\n");
#endif
while (!feof(stdin))
	{
	printf ("You have: ");
	if (str_gets(stdin, buf, sizeof(buf)) == NULL) break;
	sf = strtod(buf, &sp);
	if (sp == buf)
		{
		printf ("mcunit: usage: amount unit.\n");
		continue;
		}
	strcpy(su, sp);
	printf ("You want: ");
	if (str_gets(stdin, buf, sizeof(buf)) == NULL) break;
	printf ("%f %s -> %s\n", sf, su, buf);
	tf = unitconv (sf, su, buf);
	printf ("%f %s\n", tf, buf);
	}
exit (EXIT_SUCCESS);
}
#endif
