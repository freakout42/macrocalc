/* cur_init.c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>
#include <fcntl.h>
#if (COHERENT | linux | __CYGWIN__)
#include <term.h>
#endif

int	cur_redir;
int cur_colors;

FILE	*std_in		= NULL;
FILE	*std_out	= NULL;
FILE	*std_err	= NULL;
FILE	*std_nerr	= NULL;
FILE	*std_erread	= NULL;

#ifndef WIN32
struct termios otermio;
#endif

typedef struct attrel {
  int ccode;
  int cattr;
  int foreg;
  int backg;
} attrel;

static attrel attrels[] = {

  { COL_DEFAULT,         A_NORMAL,          -1,            -1 },            /* default */
  { COL_BLACK,           A_NORMAL,          COLOR_BLACK,   -1 },            /* black */
  { COL_RED,             A_NORMAL,          COLOR_RED,     -1 },            /* red */
  { COL_GREEN,           A_NORMAL,          COLOR_GREEN,   -1 },            /* green */
  { COL_YELLOW,          A_NORMAL,          COLOR_YELLOW,  -1 },            /* yellow */
  { COL_BLUE,            A_NORMAL,          COLOR_BLUE,    -1 },            /* blue */
  { COL_MAGENTA,         A_NORMAL,          COLOR_MAGENTA, -1 },            /* magenta */
  { COL_CYAN,            A_NORMAL,          COLOR_CYAN,    -1 },            /* cyan */
  { COL_WHITE,           A_NORMAL,          COLOR_WHITE,   -1 },            /* white */

  { TEXTCOLOR,           A_NORMAL,          0,             0 },             /* text cell */
  { EOFCOLOR,            A_NORMAL,          COLOR_GREEN,   -1 },            /* eof cell */
  { ERRORCOLOR,          A_BLINK,           COLOR_RED,     -1 },            /* error cell */
  { VALUECOLOR,          A_NORMAL,          0,             0 },             /* value cell */
  { FORMULACOLOR,        A_NORMAL,          0,             0 },             /* formula cell */
  { STRINGCOLOR,         A_NORMAL,          0,             0 },             /* string cell */
  { BLANKCOLOR,          A_NORMAL,          0,             0 },             /* blank cell */
  { COMMANDCOLOR,        A_UNDERLINE,       COLOR_BLUE,    -1 },            /* command cell */

  { HIGHLIGHTCOLOR,      A_REVERSE,         COLOR_BLUE,    COLOR_WHITE },   /* highlighted current cell */
  { HIGHLIGHTERRORCOLOR, A_REVERSE|A_BLINK, COLOR_RED,     COLOR_WHITE },   /* highlighted current cell with error */
#ifdef WIN32
  { HEADERCOLOR,         A_BOLD,            COLOR_YELLOW,  COLOR_MAGENTA }, /* column and row headers */
#else
  { HEADERCOLOR,         A_REVERSE,         COLOR_CYAN,    COLOR_BLACK },   /* column and row headers */
#endif
  { CURHEADERCOLOR,      A_BOLD,            COLOR_WHITE,   COLOR_BLUE },    /* current col/row header */
  { MARKCOLOR,           A_REVERSE,         COLOR_MAGENTA, COLOR_YELLOW },  /* marked range info */
  { AUTOCALCCOLOR,       A_REVERSE,         0,             0 },             /* autocalc info */
  { FORMDISPLAYCOLOR,    A_REVERSE,         0,             0 },             /* formula display info */
  { MESSAGECOLOR,        A_BOLD|A_BLINK,    0,             0 },             /* messages */
  { PROMPTCOLOR,         A_BOLD,            0,             0 },             /* prompt */
  { INPUTCOLOR,          A_REVERSE,         0,             0 },             /* editor */
  { CELLCONTENTSCOLOR,   A_NORMAL,          0,             0 },             /* cell content info */

  { COL_UNDEF, 0, 0, 0 }
};

int mcode2att(int colcode) {
return attrels[colcode].cattr;
}

void mcolor0(void) {
int i;

start_color();
use_default_colors();
for (i=0; i<COL_UNDEF; i++) {
//  assert(attrels[i].ccode == i);
  init_pair(i, attrels[i].foreg, attrels[i].backg);
}
}

void setcolor(int pairi) {
if (cur_colors) {
 if (attrels[pairi].foreg || attrels[pairi].backg) {
  attron(COLOR_PAIR(pairi));
 }
}
}

void uncolor(int pairi) {
if (cur_colors) {
 if (attrels[pairi].foreg || attrels[pairi].backg) {
  attroff(COLOR_PAIR(pairi));
 }
}
}

WINDOW	*cur_init (int around, int *ysz, int *xsz, unsigned char *col)
{
WINDOW		*stdscreen;
int		inpipe[2];
unsigned int	flags;
char		*devtty = "/dev/tty";
char *username;
char *lclocale;
int ysiz, xsiz;

#ifndef WIN32
struct termios termio;

/* no floating-point exception */
#ifdef SIGFPE
signal(SIGFPE, SIG_IGN);
#endif

/* user and charset environment */
#ifdef WIN32
username = getenv("USERNAME");
#define ISO_8859_15_CP 28605
/*if (IsValidCodePage(ISO_8859_15_CP) == 0) return NULL;*/
SetConsoleCP(ISO_8859_15_CP);
SetConsoleOutputCP(ISO_8859_15_CP);
#else
/* #define ISO_8859_15_CP "en_US.iso885915"
 * setenv("LC_ALL", ISO_8859_15_CP, 1);
 * setenv("LANG", ISO_8859_15_CP, 1);
 * setenv("LC_ALL", CHARSET, 1);
 */
/* ESC % @ */
username = getenv("USER");
#endif
lclocale = setlocale(LC_ALL, CHARSET);
cur_utf8 = strstr(lclocale, "UTF-8") != NULL;

if (around < 0) return NULL;

#ifndef UTF8
if (cur_utf8) return NULL;
#endif

if (around >= 1)
	{
	if ((std_in = lib_freo (devtty, "r+", stdin)) == NULL)
		{
#ifdef DEBUG
		fprintf (stderr, "cannot open /dev/tty as stdin!\n");
#endif
		return NULL;
		}
	}

if (around >= 2)
	{
	if ((std_out = lib_freo (devtty, "r+", stdout)) == NULL)
		{
#ifdef DEBUG
		fprintf (stderr, "cannot open /dev/tty as stdout!\n");
#endif
		return NULL;
		}
	}

if (around >= 3)
	{
	if (	pipe(inpipe)						||
		((std_err = lib_freo(NULL, "r+", stderr)) == NULL)	||
		(dup(inpipe[WRITE]) != STDERR_FILENO)			||
		close(inpipe[WRITE])					||
		((std_nerr = fdopen(STDERR_FILENO, "w")) == NULL) ) return NULL;
	if (stderr != std_nerr) memcpy (stderr, std_nerr, sizeof(FILE));
	setbuf (stderr, NULL);
	if ((std_erread = fdopen (inpipe[READ], "r")) == NULL) return NULL;
	fcntl (inpipe[READ], F_SETFD, 1);
	flags = fcntl (inpipe[READ], F_GETFL, 0);
	fcntl (inpipe[READ], F_SETFL, flags | O_NDELAY);
	}
#endif

cur_redir = around;

if ((stdscreen = initscr()) == NULL) return NULL;
#ifdef DEBUG
fprintf (stderr, "cur_init: %s\n", "initscr() done");
#endif
/*atexit(cur_exit);tcsetattr (0, TCSANOW, &otermio);*/

#ifdef COHERENT
back_tab		= NULL;
insert_character	= NULL;
parm_index		= NULL;
parm_up_cursor		= NULL;
column_address		= NULL;
row_address		= NULL;
#endif

#ifdef init_tabs
init_tabs		= 0;
#endif

#ifndef WIN32
  signal(SIGTSTP, SIG_IGN);
  tcgetattr (fileno(stdin), &termio); /* give me all attributes */
  otermio = termio;
  termio.c_cc[VINTR] = 0; /* ctrl-c */
  termio.c_cc[VSUSP] = 0; /* ctrl-z */
#ifdef VDSUSP
  termio.c_cc[VDSUSP] = 0; /* ctrl-y */
#endif
#ifdef VLNEXT
  termio.c_cc[VLNEXT] = 0;/* ctrl-v */
#endif
  tcsetattr (fileno(stdin), TCSANOW, &termio);
#else
/* avoid raw() and cbreak() they are nasty */
	raw();
  meta(stdscr, TRUE);
#endif
	nonl();
	noecho();
/*	cbreak(); */

#ifndef __NCURSES_H
intrflush(stdscr,FALSE);
#endif
keypad(stdscr,TRUE);

getmaxyx(stdscr, ysiz, xsiz);
*ysz = ysiz;
*xsz = xsiz;
if (*col && has_colors()) {
  cur_colors = 1;
  mcolor0();
} else {
  cur_colors = 0;
}
*col = cur_colors;

return stdscreen;
}
