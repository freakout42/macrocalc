/* cur_def.h
 * https://superjamie.github.io/2022/08/06/ncursesw
 */
#define _XOPEN_SOURCE_EXTENDED 1

#ifdef _POSIX_SOURCE
#define ARX_POSIX_SOURCE _POSIX_SOURCE
#undef _POSIX_SOURCE
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef NCURSES_WIDECHAR
#ifndef CURSES_H
#include <curses.h>
#ifdef NCURSES_WACS
#define UTF8
#define CURVARIANT w
#ifdef WIN32
#define CHARSET "English_United States.65001"
#else
#define CHARSET "en_US.UTF-8"
#endif
#else
#define CURVARIANT n
#define CHARSET "en_US.iso885915"
#endif
#endif
#define curcs1(m, i, l) #m "." #i "-" #l
#define curcs(m, i, l) curcs1(m, i, l)
#define CURSESVERSION curcs(NCURSES_VERSION_MAJOR, NCURSES_VERSION_MINOR, CURVARIANT)
#include <key2_def.h>
#else
typedef void *WINDOW;
extern WINDOW *stdscr;
#define move(y,x) wmove(stdscr,y,x)
int clrtoeol (void);
int clearok (WINDOW *,unsigned);
int delch (void);
int wmove (WINDOW *,int,int);
int deleteln (void);
int insertln (void);
int mvprintw (int,int,char *,...);
int beep (void);
#include <key_def.h>
#endif

#if (__FreeBSD__ | __APPLE__)
#include <termios.h>
#endif

#ifdef ARX_POSIX_SOURCE
#define _POSIX_SOURCE ARX_POSIX_SOURCE
#undef ARX_POSIX_SOURCE
#endif

extern int	cur_redir;
extern struct termios otermio;

#ifdef WIN32
#undef  A_UNDERLINE
#define A_UNDERLINE  A_STANDOUT
extern HANDLE stdinHandle;
#endif

#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(__STDIO_H__) || defined(_STDIO_H_) || defined(_H_STDIO) || defined(WIN32)
extern FILE	*std_in, *std_out, *std_err, *std_erread;
FILE		*cur_maco();
#endif

WINDOW *cur_init (int around, int *ysz, int *xsz, unsigned char *col);
void cur_exit (int status);
int cur_getk ();
int cur_getp (WINDOW *w);
void cur_puts(int y, int x, char *s, int w);
int cur_gets (WINDOW *w, int y, int x, int width, int att, char *s, int pos, char *legal, int maxlength, int *chg, char *(*f4edit)(char *fml, char *pos));
int cur_satt (WINDOW *w, unsigned int attrib);
int cur_syst (WINDOW *w, char *cmd, int prompt);
void cur_goll (void);
int cur_gmnu (WINDOW *w, int y, int x, int width, char *m);
void cur_rest (void);
void cur_fixt (void);
int cur_tput (int c);
void cur_refr (void);
int mcode2att(int colcode);
void mcolor0(void);
void setcolor(int pairi);
void uncolor(int pairi);
void cur_coor (int *y, int *x);
int to_utf8(char *buf, int nbuf);
#ifndef CURSES_H
int addwstr(const wchar_t *wstr);
#endif
