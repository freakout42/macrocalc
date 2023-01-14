/* mcommon.c */

#include <stdio.h>
#include <stddef.h>
#include <arx_def.h>
#include <str_def.h>
#define COMMON
#include "mc.h"
#include "mcommon.h"

char license = 0;
char *progname = NULL;
char libpath[MAXFILE+1] = ARXPATH;
char logo[] = TXTLOGO;
char macroname[MAXFILE+1] = "";
char macroexec[MAXFILE+1] = "";
char filename[MAXFILE+1] = "";
struct Range *rnames	= NULL;
CELLPTR curcell;
unsigned char colwidth[MAXCOLS];
unsigned char colstart[MAXCOLS];
char changed = FALSE;
unsigned char formdisplay = FALSE;
unsigned char autocalc = TRUE;
unsigned char autoexec = FALSE;
unsigned char borders = TRUE;
unsigned char cellcont = TRUE;
unsigned char highlight = TRUE;
unsigned char menudisp = TRUE;
unsigned char rdonly = FALSE;
unsigned char defaultformat = DEFAULTFORMAT;
unsigned char defaultwidth = DEFAULTWIDTH;
unsigned char colors = TRUE;
char currency = '$';
int screencol = 80;
int screenlin = 24;
int leftmargin = 3;
int curcelline = 0;
int inputline = 1;
int cmdline = 1;
int errorline = 2;
int borderline = 3;
int windowline = 4;
int leftcol=0, rightcol=0,
	toprow=0, topbotrow=0, windowrow=0, bottomrow=0,
	curcol=0, currow=0;
unsigned char stop = FALSE;
char *msgcommand;
int nextkey = 0;
#ifdef NOTNEEDED
static FILE *(*lib_freo_)() = lib_freo;	/* just to force include in -r linking */
static double (*str_chkd_)() = str_chkd;/* just to force include in -r linking */
#endif
#if defined(aix)
static double (*lib_2iee_)() = lib_2iee;/* just to force include in -r linking */
static double (*lib_iee2_)() = lib_iee2;/* just to force include in -r linking */
#endif
