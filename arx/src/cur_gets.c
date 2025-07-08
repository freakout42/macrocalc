/* cur_gets.c
 * Allows the user to edit a string with only certain characters allowed
 * pos == -1 signals only output no interaction
 * ESC cancels any changes
 * Returns last key
 */

#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>
#include <stdlib.h>

#ifdef UTF8
static int cur_scpy(wchar_t *t, wchar_t *s, int z) {
int i, j;
j = 0;
for (i=0; i<z; i++) {
  switch (*s) {
    case '\t': j++; s++;
    case '\0': *t++ = ' '; break;
    default:   j++; *t++ = *s++;
    }
  }
  *t = '\0';
return j;
}
static int cur_slen(wchar_t *s) { return wcslen(s); }
static void cur_adds(WINDOW *w, wchar_t *s) {
  waddwstr(w, s);
#else
static int cur_scpy(char *t, char *s, int z) {
int i, j;
j = 0;
for (i=0; i<z; i++) {
  switch (*s) {
    case '\t': j++; s++;
    case '\0': *t++ = ' '; break;
    default:   j++; *t++ = *s++;
    }
  }
  *t = '\0';
return j;
}
static int cur_slen(char *s) { return strlen(s); }
static void cur_adds(WINDOW *w, char *s) {
  waddstr(w, s);
#endif
}

int cur_gets (WINDOW *w, int y, int x, int width, int att,
	char *s, int pos, char *legal, int maxlength, int *chg,
	char *(*f4edit)(char *fml, char *pos))
{
int insert = TRUE;				/* insert mode		*/
int done = FALSE;				/* end-of-loop flag	*/
int changed = FALSE;				/* changed the string	*/
int first = TRUE;				/* first input flag	*/
int c = 0;					/* input key		*/
int sx;					/* current position x	*/
int len = strlen(s);				/* currrent string len	*/
#ifdef UTF8
wchar_t se[MAXINPUT+1];
wchar_t *so = se;					/* position in string	*/
wchar_t tmp[MAXSCREENWIDTH*2+1];			/* output string	*/
#else
char se[MAXINPUT+1];				/* my copy of string	*/
char *so = se;					/* position in string	*/
char tmp[MAXSCREENWIDTH*2+1];			/* output string	*/
#endif
int endx = x + width - 1;			/* end position		*/
char *f4pos; /* pos after f4-processing */
char f4str[MAXINPUT*2+1]; /* copy for f4 */

if (strlen(s) > MAXINPUT) return(KEY_ESC);
pos = str_pos(s, pos);
sx = x+pos;					/* current position x	*/
#ifdef UTF8
len = mbstowcs(se, s, MAXINPUT);
if (len == -1) return(KEY_ESC);
#else
strcpy(se, s);					/* save input string	*/
#endif
cur_satt(w, att);				/* set attribute	*/
while (!done)					/* input loop		*/
	{
	if (sx-x >= width)			/* behind end position?	*/
		{
		sx = endx;			/* go to end pos	*/
		so = se + pos - width + 1;
		}
	wmove(w, y, x);				/* move to print string	*/
  cur_scpy(tmp, so, width);
	cur_adds(w, tmp);			/* paint out string	*/
	if (pos==-1) break;			/* done if only paint	*/
	if (so > se && sx > x) mvwaddch (w, y, x, '<');/* signal overfl	*/
	if (cur_slen(so) > width && sx < endx) mvwaddch(w, y, endx, '>');
	wmove(w, y, sx);			/* move to cursor pos	*/
	wrefresh(w);				/* show the screen	*/
	switch (c = cur_getk (w))		/* get pressed key	*/
	 {
	 case -KEY_HOME:				/* go to start of field	*/
		pos	= 0;
		sx	= x;
		so	= se;
		break;
	 case -KEY_LL:				/* go to end of field	*/
#if (KEY_LL != KEY_END)
	 case -KEY_END:				/* go to end of field	*/
#endif
		pos	= min (len, maxlength-1);
		sx	= x + pos;
		break;
	 case -KEY_IC:				/* toggle insert mode	*/
		insert = !insert;
		break;
	 case -KEY_LEFT:				/* move left		*/
		if (pos > 0)
			{
			pos--;
			if (sx > x)	sx--;
			else		so--;
			}
		break;
	 case -KEY_RIGHT:			/* move right		*/
		if (pos < len)
			{
			pos++;
			sx++;
			}
		else done = TRUE;
		break;
	 case -KEY_BACKSPACE:			/* erase backward	*/
	 case KEY_CTRL('H'):
		if (pos > 0)
			{
			changed = TRUE;
			memmove(&se[pos - 1], &se[pos], (len - pos + 1) * sizeof(wchar_t));
			len--;
			pos--;
			if (sx > x)	sx--;
			else		so--;
			}
		break;
	 case -KEY_DC:				/* erase current	*/
		if (pos < len)
			{
			changed = TRUE;
			memmove(&se[pos], &se[pos + 1], (len - pos) * sizeof(wchar_t));
			len--;
			}
		else if (len==0) {c = KEY_CTRL('D'); done = TRUE;}
		break;
	 case KEY_ESC:				/* cancel editing	*/
	 case KEY_CTRL('C'):
		wmove(w, y, x);
#ifdef UTF8
    mbstowcs(se, s, MAXINPUT);
#endif
    cur_scpy(tmp, se, width);
		cur_adds(w, tmp);
		changed = FALSE;
		done = TRUE;
		break;
	 case -KEY_ENTER:
		done = TRUE;
		break;
	 case '$':
	 case -KEY_F(4):
		if (f4edit) {
#ifdef UTF8
     wcstombs(f4str, se, MAXINPUT*2);
#else
     strcpy(f4str, se);
#endif
     if ((f4pos = f4edit(f4str, f4str+pos))) {
#ifdef UTF8
      mbstowcs(se, f4str, MAXINPUT);
#else
      strcpy(se, f4str);
#endif
			len = cur_slen(se);
			pos = f4pos - f4str;
			sx	= x + pos;
			changed = TRUE;
     }
		}
		break;
	 default:				/* char input?		*/
		if ( ((c >= ' ') && (c <= '~')) || (c == '\t') || (c >= 0x80) ) {
		    if (    ((legal[0] == 0)	/* legal input?		*/
			 || (strchr(legal, c) != NULL)) )
			{
			changed = TRUE;
			if (pos==0 && first) len = 0;/* erase on pos0	*/
			if (len < maxlength)
				{
				if (insert)
					{
					memmove(se+pos+1, se+pos, (len - pos + 1) * sizeof(wchar_t));
					*(se+pos) = ' ';
					len++;
					}
				else if (pos >= len)
					len++;
				}
			se[pos] = c;
			if (len < maxlength || pos < len-1)
				{
				pos++;
				sx++;
				}
			}
		    else beep();		/* no valid char	*/
		    }
		else done = TRUE;
	 } /* switch */
	first = FALSE;
	se[len] = '\0';
	}
cur_satt(w, 0);
if (changed) {
#ifdef UTF8
len = wcstombs(s, se, MAXINPUT*2);
#else
strcpy(s, se);
#endif
}
if (chg) *chg = changed;
return(c);
}
