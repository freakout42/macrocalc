/* cur_def.h */

#ifdef _POSIX_SOURCE
#define ARX_POSIX_SOURCE _POSIX_SOURCE
#undef _POSIX_SOURCE
#endif

#ifdef WIN32
#include <windows.h>
#include <ncurses/ncurses.h>
#endif

#ifndef CURSES_H
#include <curses.h>
#endif

#if (__FreeBSD__ | __APPLE__)
#include <termios.h>
#endif

#ifdef ARX_POSIX_SOURCE
#define _POSIX_SOURCE ARX_POSIX_SOURCE
#undef ARX_POSIX_SOURCE
#endif

#include <key2_def.h>

extern int	cur_redir;
extern struct termios otermio;

#ifdef WIN32
#undef  A_UNDERLINE
#define A_UNDERLINE  A_STANDOUT
#endif

#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(__STDIO_H__) || defined(_STDIO_H_) || defined(_H_STDIO) || defined(WIN32)
extern FILE	*std_in, *std_out, *std_err, *std_erread;
FILE		*cur_maco();
#endif

WINDOW *cur_init (int around);
void cur_exit (int status);
int cur_getk (WINDOW *w);
int cur_getp (WINDOW *w);
int cur_gets (WINDOW *w, int y, int x, int width, int att, char *s, int pos, char *legal, int maxlength, int *chg, char *(*f4edit)(char *fml, char *pos));
int cur_satt (WINDOW *w, unsigned int attrib);
int cur_syst (WINDOW *w, char *cmd, int prompt);
void cur_goll (void);
int cur_gmnu (WINDOW *w, int y, int x, int width, char *m);
void cur_rest (void);
void cur_fixt (void);
int cur_tput (int c);
