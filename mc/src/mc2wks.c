/* $Id: mc2wks.c,v 1.14 2006/09/27 06:22:46 axel Exp $
 */

#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include "mc2wks.h"
#include "mcbool.h"
#include "mcver.h"
#include "mcelldef.h"
#include "mcommon.h"
#include "mcell.h"
#include "mcutil.h"
#include "mcfileio.h"

static char rcsinfo[] = "$""Id: mc2wks " MCREL "." MCVER " " PLATFORM CCOMPILER " $";

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

int warn = 0;
int erro = 0;

static char opts[] = "78rc:d:f:vx";

static void usage (void)
{
fprintf (stderr, "usage: mc2wks [-%s]\n", opts);
exit (EXIT_FAILURE);
}

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

int bits = DEFAULTBITS;
int reverse = FALSE;
int loaded;
int license;

progname = "mc2wks";
license = lib_akey ("000000", FALSE);
tzset(); /* set timezone */
while ((c = getopt (argc, argv, opts)) != EOF)
	switch (c)
	 {
	 case '7':
	 	bits		= 7;
		break;
	 case '8':
	 	bits		= 8;
		break;
	 case 'r':
	 	reverse		= TRUE;
		break;
	 case 'x':
		autoexec	= TRUE;
		break;
	 case 'c':
		defaultwidth	= max (atoi(optarg), MINCOLWIDTH);
		break;
	 case 'd':
		defaultformat	&= FORMATM;
		defaultformat	|= PLACES & atoi(optarg);
		break;
	 case 'f':
		defaultformat	&= PLACES;
		switch (*optarg)
		 {
		 case 'F':	defaultformat	|= FIXED;	break;
		 case 'S':	defaultformat	|= SCIENTIFIC;	break;
		 case ',':	defaultformat	|= COMMA;	break;
		 case 'C':	defaultformat	|= CURRENCY;	break;
		 case 'D':	defaultformat	= SPECIAL|DATE;	break;
		 case 'T':	defaultformat	= SPECIAL|TIME;	break;
		 case 'G':	defaultformat	= SPECIAL|DEFAULT; break;
		 default:	usage();
		 }
		break;
	 case 'v':
		fprintf(stderr, "%s\n", rcsinfo);
		/*FALLTHRU*/
	 default:
		usage();
	 }
for (; optind < argc; optind++)
	{
	usage();
	}

memset (colwidth, defaultwidth, sizeof(colwidth));
inithash();
if (reverse==TRUE)
	{
/*	lics2ibm (bits); */
	fromwks (stdin);
	savefile (stdout, FULL);
	}
else
	{
	loaded	= loadfile (stdin);
	switch (loaded)
	 {
	 case RET_FATAL:
		fatal (EXIT_FATAL, msg[MSGNOMICROCALC]);
		/*FALLTHRU*/
	 case RET_ERROR:
		fatal (EXIT_FAILURE, msg[MSGFILELOMEM]);
		/*FALLTHRU*/
	 case RET_SUCCESS:
		break;
	 }
/*	ibm2lics (bits); */
	towks (stdout);
	}

if (warn)
	{
	fprintf (stderr, "mc2wks: warning.\n"
			"not translatable LICS-codes in sourcefile.\n"
			"replaced by ASCII-codes.\n"
			"when retranslating the file the LICS-codes get lost.\n"
		);
	}

if (erro)
	{
	fprintf (stderr, "mc2wks: warning.\n"
			"not translatable LICS-codes in sourcefile.\n"
			"when retranslating the file the LICS-codes get lost.\n"
		);
	}

return EXIT_SUCCESS;
}
