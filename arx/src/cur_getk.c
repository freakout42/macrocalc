/* cur_getk.c */

#include <stdio.h>
#include <arx_def.h>
#include <cur_def.h>

static FILE *macro = NULL;

FILE *cur_maco (char *mf)
{
macro = popen (mf, "r");
return macro;
}

int cur_getk ()
{
int ch;

if (macro != NULL)
	{
	if (fscanf (macro, "%d\n", &ch) == 1) return ch;
	pclose (macro);
	macro	= NULL;
	}
return cur_getp(stdscr);
}

int cur_getp (WINDOW *w)
{
int ch;
int keycode;
#ifdef UTF8
wint_t keypress = { 0 };
#endif

wrefresh (w);
#ifdef UTF8
keycode = (wget_wch(w, &keypress) == KEY_CODE_YES) ? -1 : 1;
ch = keypress;
#else
ch = wgetch (w);
keycode = ch > 255 ? -1 : 1;
#endif
#ifdef WIN32
if (ch < 0) ch = 256 + ch;
#endif
switch (ch)
 {
 case KEY_BS:        return -KEY_BACKSPACE;
 case KEY_RETURN:
 case KEY_RRETURN:   return -KEY_ENTER;
 case KEY_LL:        return -KEY_END;
 case KEY_F0:        return -KEY_F(10);
 case KEY_CTRL('A'): return -KEY_HOME;
 case KEY_CTRL('B'): return -KEY_LEFT;
 case KEY_CTRL('C'): return -KEY_CANCEL;
 case KEY_CTRL('D'): return -KEY_DC;
 case KEY_CTRL('E'): return -KEY_END;
 case KEY_CTRL('F'): return -KEY_RIGHT;
 case KEY_CTRL('G'): return -KEY_BTAB;
 case KEY_CTRL('N'): return -KEY_DOWN;
 case KEY_CTRL('O'): return -KEY_IC;
 case KEY_CTRL('P'): return -KEY_UP;
 case KEY_CTRL('R'): return -KEY_PPAGE;
 case KEY_CTRL('U'): return -KEY_F(2);
 case KEY_CTRL('V'): return -KEY_NPAGE;
 case KEY_CTRL('Y'): return -KEY_CANCEL;
 case KEY_CTRL('Z'): return -KEY_F(8);
 }
return ch * keycode;
}
