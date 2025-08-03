/* mc.c */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcfileio.h"
#include "mcprint.h"
#include "mcinter.h"
#include "mcrmacro.h"
#include "mcmessag.h"
#include "mcgetmsg.h"
#include "mcfileio.h"
#include "mccalc.h"
#include "mcunit.h"
#include "mcutil.h"

char rcsinfo[] = "$""Id: mc " MCREL "." MCVER " " PLATFORM CCOMPILER " $";
static char opts[] = "abc:d:ef:ghklmn:o:pqrsuvxy:";

static void usage (void)
{
fprintf (stderr, "usage: %s [-%s] [file]\n", progname, opts);
exit (EXIT_SYNTAX);
}

int main (int argc, char *argv[])
{
int c = 0;
char *arxpath;
#define MAXRCARGS 8
char *mcrc;
int rcargc;
char *argv2[MAXRCARGS];
char runpath[MAXFILE+1];
char *p;
int l;
char m[MAXSCREENWIDTH+1];

license = lib_akey (logo, FALSE);
progname = strrchr(argv[0], PATHSEP);
#ifdef WIN32
if (progname==NULL) {
  strcpy(libpath, ".");
  progname = argv[0];
} else {
  *progname++ = '\0';
  strncpy(libpath, argv[0], MAXFILE-1);
}
#else
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
#ifndef WIN32
  setenv("ARX", libpath, 0);
#endif
} else {
  strcpy (libpath, arxpath);
}
#endif
tzset(); /* set timezone */
/* while ((c = getopt (argc, argv, opts)) != EOF) added $MACROCALC */
rcargc = 1; /* ==1 : no env options */
if ((mcrc = getenv("MACROCALC"))) {
argv2[0] = argv[0];
argv2[rcargc] = strtok(mcrc, " ");
while (argv2[rcargc] && ++rcargc < MAXRCARGS)
  argv2[rcargc] = strtok(NULL, " ");
argv2[rcargc] = NULL;
}
#ifndef UNITPIPE
unitinit();
#endif
while (1) {
  if (rcargc > 1) {
    c = getopt (rcargc, argv2, opts);
    if (c == EOF) {
      rcargc = 1;
      optind = 0;
    }
  }
  if (rcargc == 1) c = getopt (argc, argv, opts);
  if (c == EOF) break;
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
	 case 'e':
		borderline = 0; /* -blms */
		menudisp = FALSE;
		cellcont = FALSE;
		/*FALLTHRU*/
	 case 'b':
		borders		= FALSE;
		break;
	 case 'k':
		colors = FALSE;
		break;
	 case 's':
		cellcont	= FALSE;
		break;
	 case 'g':
		markfull = TRUE;
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
		currency	= strdup(optarg);
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
#ifdef UNITPIPE
		if (unitinit())
			{
			fprintf (stderr, "mc: cannot start mcunits.\n");
			exit (EXIT_FAILURE);
			}
#endif
		break;
	 case 'p':
	 case 'q':
    cur_init(-1, NULL, NULL, NULL);
		markfull = TRUE;
		initvars();
		switch (loadfile (stdin))
		 {
		 case RET_SUCCESS:
			if (*macroname)	runmacro (NOPROMPT);
			break;
		 case RET_ERROR:
			getmessage (m, MSGFILELOMEM);
			fprintf (stderr, "mc: %s.\n", m);
			exit (EXIT_FAILURE);
			/*FALLTHRU*/
		 case RET_FATAL:
			getmessage (m, MSGNOMICROCALC);
			fprintf (stderr, "mc: %s.\n", m);
			exit (EXIT_FATAL);
		 }
    recalcworksheet();
		if (c=='p') printfile (stdout, FALSE); else csvfile (stdout);
		exit (EXIT_SUCCESS);
	 case 'v':
		fprintf(stderr, "%s\n%s\n", lib_veid(), rcsinfo);
		/*FALLTHRU*/
	 default:
		usage();
	 }
}
*filename = '\0';
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
