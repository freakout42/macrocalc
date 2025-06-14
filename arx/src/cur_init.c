/* cur_init.c */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <arx_def.h>
#include <cur_def.h>
#include <fcntl.h>
#if (COHERENT | linux | __CYGWIN__)
#include <term.h>
#endif

int	cur_redir;

FILE	*std_in		= NULL;
FILE	*std_out	= NULL;
FILE	*std_err	= NULL;
FILE	*std_nerr	= NULL;
FILE	*std_erread	= NULL;

#ifndef WIN32
struct termios otermio;
#endif

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
  mcolor0();
} else {
  *col = FALSE;
}

return stdscreen;
}
