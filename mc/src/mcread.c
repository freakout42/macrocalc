/* mcread.c 1.10 2006/09/27 06:22:46 axel Exp */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arx_def.h>
#include "mcver.h"
#include "mcelldef.h"
#include "mcommon.h"
#include "mcell.h"
#include "mcutil.h"
#include "mcmisc.h"
#include "mcelladr.h"
#include "mcfileio.h"
#include "mcpform.h"

char rcsinfo[] = "$""Id: mcread " MCREL "." MCVER " " PLATFORM CCOMPILER " $";

#define	DEFAULTBITS 8

#define makemsg(n,s)	n,
enum	{
#include "mcmsg.h"
	MSGEND2
	};
#undef makemsg
#define makemsg(n,s)	#s,
char	*msg[]	=
	{
#include "mcmsg.h"
	NULL
	};

static char opts[] = "hr:v";

void usage(void)
{
fprintf (stderr, "mcread - read mc-file formulas - v1.0\n");
fprintf (stderr, "-- /ARX cleanware (c)1997 Axel Reinhold --\n\n");
fprintf (stderr, "usage: mcread [-r range] <mc-file\n");
fprintf (stderr, "\toptions:\n");
fprintf (stderr, "\t -r range    select range from worksheet\n");
exit (EXIT_FAILURE);
}

extern int origcol, origrow;	/* original column		*/

void errormsg (int n)
/* Prints an error message */
{
fatal (EXIT_FATAL, msg[n]);
} /* errormsg */

int main (int argc, char *argv[])
{
extern int optind;
extern char *optarg;
int c;

int loaded;
#ifdef LICENSED
int license;
#endif
char *rangespec = NULL;
struct Range range;

progname = "mcread";
#ifdef LICENSED
license = lib_akey ("000000", FALSE);
#endif
tzset(); /* set timezone */
while ((c = getopt (argc, argv, opts)) != EOF)
	switch (c)
	 {
	 case 'r':
		rangespec = optarg;
		break;
	 case 'v':
		fprintf(stderr, "%s\n", rcsinfo);
		/*FALLTHRU*/
	 case 'h':
		/*FALLTHRU*/
	 default:
		usage();
	 }
for (; optind < argc; optind++)
	{
	usage();
	}

memset(colwidth, defaultwidth, sizeof(colwidth));
inithash();
loaded	= loadfile(stdin);
switch (loaded) {
 case RET_FATAL:
	fatal (EXIT_FATAL, msg[MSGNOMICROCALC]);
	/*FALLTHRU*/
 case RET_ERROR:
	fatal (EXIT_FAILURE, msg[MSGFILELOMEM]);
	/*FALLTHRU*/
 }

if (rangespec != NULL) {
	if (rangeadr(rangespec, &range) != strlen(rangespec)) {
#ifdef DEBUG
fprintf(stderr, "rangeadr(rangespec, &range)=%d strlen(rangespec)=%d "
		"col0=%d row0=%d col1=%d row1=%d\n",
		rangeadr(rangespec, &range), strlen(rangespec),
		range.adr[0].col, range.adr[0].row,
		range.adr[1].col, range.adr[1].row);
#endif
		fatal (EXIT_FATAL, msg[MSGUNRANGE]);
		usage();
		}
	omarkcol = range.adr[0].col;
	omarkrow = range.adr[0].row;
	markcol = range.adr[1].col;
	markrow = range.adr[1].row;
	}

printformula(stdout);

return EXIT_SUCCESS;
}
