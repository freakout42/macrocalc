/* cur_getk.c */

#include <stdio.h>
#include <arx_def.h>
#include <str_def.h>
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
if (cur_utf8) {
#ifdef WIN32
  DWORD n = 0;
  int uc, sc, ck;
  ch = 0;
  INPUT_RECORD ir[1];
  while ( ch == 0) {
    ReadConsoleInputW(stdinHandle, ir, 1, &n);
    if (ir[0].EventType & KEY_EVENT) {
     if (ir[0].Event.KeyEvent.bKeyDown) {
      sc = ir[0].Event.KeyEvent.wVirtualScanCode;
      ck = ir[0].Event.KeyEvent.dwControlKeyState;
      uc = ir[0].Event.KeyEvent.uChar.UnicodeChar;
      if (uc == 0) {
       if (!(ck & (SHIFT_PRESSED | LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED | LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED )))
        if (sc != ':') {
          keycode = -1;
          switch (sc) {
            case 0x3B: ch = KEY_F(1); break;
            case 0x3C: ch = KEY_F(2); break;
            case 0x3D: ch = KEY_F(3); break;
            case 0x3E: ch = KEY_F(4); break;
            case 0x3F: ch = KEY_F(5); break;
            case 0x40: ch = KEY_F(6); break;
            case 0x41: ch = KEY_F(7); break;
            case 0x42: ch = KEY_F(8); break;
            case 0x43: ch = KEY_F(9); break;
            case 0x44: ch = KEY_F(10); break;
            case 0x45: ch = KEY_F(11); break;
            case 0x46: ch = KEY_F(12); break;
            case 0x47: ch = KEY_HOME; break;
            case 0x48: ch = KEY_UP; break;
            case 0x49: ch = KEY_PPAGE; break;
            case 0x4B: ch = KEY_LEFT; break;
            case 0x4D: ch = KEY_RIGHT; break;
            case 0x4F: ch = KEY_END; break;
            case 0x50: ch = KEY_DOWN; break;
            case 0x51: ch = KEY_NPAGE; break;
            case 0x52: ch = KEY_IC; break;
            case 0x53: ch = KEY_DC; break;
          }
        }
      } else {
        keycode = 1;
        ch = uc;
      }
     }
    }
  }
#else
keycode = (wget_wch(w, &keypress) == KEY_CODE_YES) ? -1 : 1;
ch = keypress;
#endif /* WIN32 */
} else
#endif /* UTF8 */
{
ch = wgetch (w);
keycode = ch > 255 ? -1 : 1;
}
#ifndef UTF8
#ifdef WIN32
if (ch < 0) ch = 256 + ch;
#endif
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
