/* mc.c */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arx_def.h>
#include "mc.h"
#include "mcfileio.h"
#include "mcprint.h"
#include "mcinter.h"
#include "mcrmacro.h"
#include "mcmessag.h"
#include "mcfileio.h"
#include "mccalc.h"
#include "mcunit.h"
#include "mcutil.h"

static char rcsinfo[] = "$""Id: mc " MCREL "." MCVER " " PLATFORM CCOMPILER " " __DATE__ " $";
static char opts[] = "abc:d:f:hlmn:o:pqrsuvxy:";

static void usage (void)
{
fprintf (stderr, "usage: %s [-%s] [file]\n", progname, opts);
exit (EXIT_SYNTAX);
}

int main (int argc, char *argv[])
{
extern int optind;
extern char *optarg;
int c;
char *arxpath;
char runpath[MAXFILE+1];
char *p;
int l;

license = lib_akey (logo, FALSE);
progname = strrchr(argv[0],'/');
if (progname==NULL) progname=argv[0]; else progname++;
if ((arxpath = getenv("ARX")) == NULL) {
  if (progname == argv[0]) {
    if ((arxpath = lib_path (getenv("PATH"), progname, X_OK)) != NULL && strlen(arxpath) < MAXFILE) {
      strcpy(runpath, arxpath);
      if ((p = strrchr(runpath,'/')) != NULL) {
        *p = '\0';
        if ((p = strrchr(runpath,'/')) != NULL) {
          *p = '\0';
          if (lib_path (runpath, "lib/mcmnu.rdb", W_OK)) {
            strcpy (libpath, runpath);
          }
        }
      }
    }
  } else {
    l = progname - argv[0];
    if (l < MAXFILE) {
      strncpy (libpath, argv[0], l-1);
      libpath[l-1] = '\0';
      strcat (libpath, "/..");
    }
  }
  setenv("ARX", libpath, 0);
} else {
  strcpy (libpath, arxpath);
}
tzset(); /* set timezone */
while ((c = getopt (argc, argv, opts)) != EOF)
	switch (c)
	 {
	 case 'a':
		autocalc	= FALSE;
		break;
	 case 'x':
		autoexec	= TRUE;
		break;
	 case 'n':
		strcpy (macroexec, optarg);
		break;
	 case 'o':
		strcpy (macroname, optarg);
		break;
	 case 'b':
		borders		= FALSE;
		break;
	 case 's':
		cellcont	= FALSE;
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
		 case 'C':	defaultformat	|= CURRENCY;	break;
		 case ',':	defaultformat	|= COMMA;	break;
		 case 'P':	defaultformat	|= PERCENT;	break;
		 case '%':	defaultformat	|= PERCENT;	break;
		 case 'D':	defaultformat	= SPECIAL|DATE;	break;
		 case 'T':	defaultformat	= SPECIAL|TIME;	break;
		 case 'G':	defaultformat	= SPECIAL|DEFAULT; break;
		 default:	usage();
		 }
		break;
	 case 'y':
		currency	= *optarg;
		break;
	 case 'l':
		borderline	= 0;
		break;
	 case 'h':
		highlight	= FALSE;
		break;
	 case 'm':
		menudisp	= FALSE;
		break;
	 case 'r':
		rdonly	= TRUE;
		break;
	 case 'u':
		if (unitinit())
			{
			fprintf (stderr, "mc: cannot start mcunits.\n");
			exit (EXIT_FAILURE);
			}
		break;
	 case 'p':
	 case 'q':
		initvars();
		switch (loadfile (stdin))
		 {
		 case RET_SUCCESS:
			if (*macroname)	runmacro (NOPROMPT);
			break;
		 case RET_ERROR:
			errormsg (MSGFILELOMEM);
			exit (EXIT_FAILURE);
			/*FALLTHRU*/
		 case RET_FATAL:
			errormsg (MSGNOMICROCALC);
			exit (EXIT_FATAL);
		 }
    recalcworksheet();
		if (c=='p') printfile (stdout, FALSE); else csvfile (stdout);
		exit (EXIT_SUCCESS);
	 case 'v':
		fprintf(stderr, "%s\n", rcsinfo);
		/*FALLTHRU*/
	 default:
		usage();
	 }
switch (argc-optind)
 {
 case 0:	break;
 case 1:	strcpy (filename, argv[optind]); break;
 default:	usage();
 }
mcinter();
unitclose();
return EXIT_SUCCESS;
}
