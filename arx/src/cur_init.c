/* $Id: cur_init.c,v 1.23 2022/09/29 05:51:36 axel Exp $
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <arx_def.h>
#include <cur_def.h>
#include <fcntl.h>
#if (COHERENT | linux)
#include <term.h>
#endif

int	cur_redir;

FILE	*std_in		= NULL;
FILE	*std_out	= NULL;
FILE	*std_err	= NULL;
FILE	*std_nerr	= NULL;
FILE	*std_erread	= NULL;

WINDOW	*cur_init (int around)
{
WINDOW		*stdscreen;
int		inpipe[2];
unsigned int	flags;
char		*devtty = "/dev/tty";
struct termios termio;

/* no floating-point exception */
#ifdef SIGFPE
signal(SIGFPE, SIG_IGN);
#endif
#ifdef SIGINT
signal(SIGINT, SIG_IGN);
#endif

cur_redir = around;

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

if ((stdscreen = initscr()) == NULL) return NULL;
#ifdef DEBUG
fprintf (stderr, "cur_init: %s\n", "initscr() done");
#endif

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

/*atexit(cur_exit);*/

  tcgetattr (fileno(stdin), &termio);
  termio.c_cc[VINTR] = 0;
  termio.c_cc[VSUSP] = 0;
#ifdef VLNEXT
  termio.c_cc[VLNEXT] = 0;
#endif
  tcsetattr (fileno(stdin), TCSANOW, &termio);

#ifdef USERAW
	raw();
#else
	noecho();
	cbreak();
	nonl();
#endif

#ifndef __NCURSES_H
intrflush(stdscr,FALSE);
#endif
keypad(stdscr,TRUE);

return stdscreen;
}
