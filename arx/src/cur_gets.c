/* cur_gets.c
 * Allows the user to edit a string with only certain characters allowed
 * pos == -1 signals only output no interaction
 * ESC cancels any changes
 * Returns last key
 */

#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>

int cur_gets (WINDOW *w, int y, int x, int width, int att,
	char *s, int pos, char *legal, int maxlength, int *chg,
	char *(*f4edit)(char *fml, char *pos))
{
int insert = TRUE;				/* insert mode		*/
int done = FALSE;				/* end-of-loop flag	*/
int changed = FALSE;				/* changed the string	*/
int first = TRUE;				/* first input flag	*/
int c = 0;					/* input key		*/
int sx = x+pos;					/* current position x	*/
int len = strlen(s);				/* currrent string len	*/
char *tab;					/* position of tab	*/
char se[MAXINPUT];				/* my copy of string	*/
char *so = se;					/* position in string	*/
char tmp[MAXSCREENWIDTH+1];			/* output string	*/
int endx = x + width - 1;			/* end position		*/
char *f4pos; /* pos after f4-processing */
if (strlen(s) > MAXINPUT) return(KEY_ESC);
strcpy(se, s);					/* save input string	*/
cur_satt(w, att);				/* set attribute	*/
while (!done)					/* input loop		*/
	{
	if (sx-x >= width)			/* behind end position?	*/
		{
		sx = endx;			/* go to end pos	*/
		so = se + pos - width + 1;
		}
	wmove(w, y, x);				/* move to print string	*/
	snprintf(tmp, width, "%-*.*s", width, width, so);/* output string	*/
	tmp[width] = '\0';			/* cut to width		*/
	while ((tab = strchr(tmp,'\t')) != NULL) *tab = ' '; /* tab era	*/
	waddstr(w, tmp);			/* paint out string	*/
	if (pos==-1) break;			/* done if only paint	*/
	if (so > se && sx > x) mvwaddch (w, y, x, '<');/* signal overfl	*/
	if ((int)strlen(so) > width && sx < endx) mvwaddch(w, y, endx, '>');
	wmove(w, y, sx);			/* move to cursor pos	*/
	wrefresh(w);				/* show the screen	*/
	switch (c = cur_getk (w))		/* get pressed key	*/
	 {
	 case KEY_HOME:				/* go to start of field	*/
		pos	= 0;
		sx	= x;
		so	= se;
		break;
	 case KEY_LL:				/* go to end of field	*/
#if (KEY_LL != KEY_END)
	 case KEY_END:				/* go to end of field	*/
#endif
		pos	= min (len, maxlength-1);
		sx	= x + pos;
		break;
	 case KEY_IC:				/* toggle insert mode	*/
		insert = !insert;
		break;
	 case KEY_LEFT:				/* move left		*/
		if (pos > 0)
			{
			pos--;
			if (sx > x)	sx--;
			else		so--;
			}
		break;
	 case KEY_RIGHT:			/* move right		*/
		if (pos < len)
			{
			pos++;
			sx++;
			}
		else done = TRUE;
		break;
	 case KEY_BACKSPACE:			/* erase backward	*/
	 case KEY_CTRL('H'):
		if (pos > 0)
			{
			changed = TRUE;
			memmove(&se[pos - 1], &se[pos], len - pos + 1);
			len--;
			pos--;
			if (sx > x)	sx--;
			else		so--;
			}
		break;
	 case KEY_DC:				/* erase current	*/
		if (pos < len)
			{
			changed = TRUE;
			memmove(&se[pos], &se[pos + 1], len - pos);
			len--;
			}
		else if (len==0) {c = KEY_CTRL('D'); done = TRUE;}
		break;
	 case KEY_ESC:				/* cancel editing	*/
	 case KEY_CTRL('C'):
		wmove(w, y, x);
		snprintf(tmp, width, "%-*.*s", width, width, s);
		tmp[width] = '\0';
		while ((tab = strchr(tmp,'\t')) != NULL) *tab = ' ';
		waddstr(w, tmp);
		changed = FALSE;
		done = TRUE;
		break;
	 case '$':
	 case KEY_F(4):
		if (f4edit && (f4pos = f4edit(se, se+pos))) {
			len = strlen(se);
			pos = f4pos - se;
			sx	= x + pos;
			changed = TRUE;
			break;
		}
	 default:				/* char input?		*/
		if     (    ((c >= ' ') && (c <= '~'))
			 || (c == '\t')
			 || ((c >= 0x80) && (c <= 0xff)) )
		    {
		    if (    ((legal[0] == 0)	/* legal input?		*/
			 || (strchr(legal, c) != NULL)) )
			{
			changed = TRUE;
			if (pos==0 && first) len = 0;/* erase on pos0	*/
			if (len < maxlength)
				{
				if (insert)
					{
					memmove(se+pos+1, se+pos, len - pos +1);
					*(se+pos) = ' ';
					len++;
					}
				else if (pos >= len)
					len++;
				}
			se[pos] = (char)c;
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
if (changed) strcpy(s, se);
if (chg) *chg = changed;
return(c);
}
