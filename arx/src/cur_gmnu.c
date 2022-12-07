/* cur_gmnu.c 1.7 1997/02/07 19:28:08 axel
 * curses get menu item
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <arx_def.h>
#include <cur_def.h>

static void clear_menu(WINDOW *w, int y, int x, int width) {
wmove (w, y, x);
wprintw (w, "%*s", width, "");
wmove (w, y, x);
}

int cur_gmnu (WINDOW *w, int y, int x, int width, char *menu)
/* Allows the user to select from a lotus-style menu
 * Returns last key pressed or 0 for ESC or -1 to -n for selected menu item
 */
{
#define MAXMENUITEM 255
int	code = 0;		/* current code number */
int	active;			/* active code number */
int	maxcode;		/* last code number */
int	c;			/* loop code */
int	key;			/* key pressed */
int	skey = 0;		/* returned key */
char	*s;			/* loop pointer in menu */
char	*npos;			/* next menu item space pos */
char	*lpos;			/* last char in menu */
int	cy=0, cx=0;			/* screen positions */
char	*cmenu;			/* start pos for displayed menu */
char	item[MAXMENUITEM];	/* menu char array */
char	*pos[MAXMENUITEM];	/* menu pos array */

memset(item, '\0', MAXMENUITEM);
cmenu = menu;			/* start with left side of menu */
lpos = menu+strlen(menu)-1;	/* last char in menu */
do	{
	wmove (w, y, x);	/* move to display start */
	active = 0;		/* first menu item active */
	pos[0] = menu;		/* first item starts at menu start */
	for (s=menu; *s; s++)	/* walk through whole menu */
		{
		/* menu char is first uppercase or punctuation char */
		if (isupper(*s) || ispunct(*s)) {
			item[active] = *s;	/* remember menu char */
			cur_satt (w, A_UNDERLINE);
			if (active==code) getyx (w, cy, cx);
			}
		else if (active==code)
			cur_satt (w, A_REVERSE);
		else	cur_satt (w, A_NORMAL);
		if (*s == ' ') {
			cur_satt (w, A_NORMAL);
			active++;
			item[active] = '\1';
			pos[active] = s+1;
			}
		if (s>=cmenu && s-cmenu<width) waddch (w, *s);
		}
	maxcode	= active;
	cur_satt (w, A_NORMAL);
	wmove (w, cy, cx);
	key = cur_getk (w);
	if (key<=0xff && isalpha(key)) key = toupper (key);
	switch (key)
	 {
	 case KEY_RIGHT:
		if (++code > maxcode) {
			code = 0;
			}
		break;
	 case KEY_LEFT:
		if (code > 0) code--;
		else code = maxcode;
		break;
	 case KEY_ESC:
	 	skey = KEY_ESC;
		break;	 	
	 default:
	 	for (c=0; c<MAXMENUITEM; c++)
			if (item[c]==key) {
				code = c;
			 	skey = KEY_ENTER;
				break;
				}
		break;
	 case KEY_ENTER:
	 	skey = KEY_ENTER;
		break;
	 }
	npos = strchr(pos[code], ' ');
	if (npos==NULL) npos = lpos; else npos--;
	if (code==0)
		cmenu = menu;
	else if (pos[code]<cmenu) {
		if (npos-width<menu) cmenu = menu;
		else cmenu = strchr(npos-width, ' ')+1;
		}
	else if (npos>=cmenu+width) {
		if (pos[code]+width>lpos) {
			cmenu = strchr(lpos-width, ' ')+1;
			clear_menu(w, y, x, width);
			}
		else cmenu = pos[code];
		}
	} while (skey==0);
clear_menu(w, y, x, width);
return skey==KEY_ENTER ? -(code+1) : skey==KEY_ESC ? 0 : skey;
}
