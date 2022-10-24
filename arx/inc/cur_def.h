/* $Id: cur_def.h,v 1.23 2022/10/08 15:38:08 axel Exp $
 */

#ifdef _POSIX_SOURCE
#define ARX_POSIX_SOURCE _POSIX_SOURCE
#undef _POSIX_SOURCE
#endif

#ifndef CURSES_H
#include <curses.h>
#endif

#if (__FreeBSD__ | __APPLE__)
#include <termios.h>
/*
struct  termios  ostate;
struct  termios  nstate;
 */
#else
/*
#include <termio.h>
struct  termio  ostate;
struct  termio  nstate;
 */
#endif

#ifdef ARX_POSIX_SOURCE
#define _POSIX_SOURCE ARX_POSIX_SOURCE
#undef ARX_POSIX_SOURCE
#endif

#ifdef COHERENT
/*#define CURSESBUG*/
#ifndef __NCURSES_H
#define wgetch cur_getc
#endif
#endif

#ifdef strchr
#undef strchr
#undef strrchr
#endif

#include <key2_def.h>

extern int	cur_redir;

#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(__STDIO_H__) || defined(_STDIO_H_) || defined(_H_STDIO)
extern FILE	*std_in, *std_out, *std_err, *std_erread;
FILE		*cur_maco();
#endif

WINDOW *cur_init (int around);
void cur_exit (int status);
int cur_getk (WINDOW *w);
int cur_getp (WINDOW *w);
int cur_gets (WINDOW *w, int y, int x, int width, int att,
	char *s, int pos, char *legal, int maxlength, int *chg);
int cur_satt (WINDOW *w, unsigned int attrib);
int cur_syst (WINDOW *w, char *cmd, int prompt);
void cur_goll (void);
int cur_gmnu (WINDOW *w, int y, int x, int width, char *m);
void cur_rest (void);
void cur_fixt (void);
int cur_tput (int c);
