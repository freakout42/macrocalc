/* mcgetmnu.c */

#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcgetmnu.h"

#ifdef MSGINLINE
static char *mnu[] = {
#include "mcmnu2.h"
NULL };
#else
extern char *mnu[];
#endif

int readmenus (void)
{
#ifdef MSGINLINE
return 0;
#else
char menufilename[MAXFILE+1];
char menuline[MAXSCREENWIDTH+1];
FILE *menu;
int i = 0;

sprintf (menufilename, "%s/lib/mcmnu.rdb", libpath);
if ((menu = fopen (menufilename, "r")))
	{
	while (str_gets (menu, menuline, sizeof (menuline)))
		{
		if (!(mnu[i++] = strdup (menuline)))
			{
			return RET_ERROR;
			}
		}
	fclose (menu);
	}
else	return RET_ERROR;
return RET_SUCCESS;
#endif
}

int getmenu (int n)
/* Gets a menu item */
{
int ch;

while ((ch = cur_gmnu (stdscr, cmdline, 0, SCREENWIDTH, mnu[n])) > 0);
return ch == 0 ? -1 : -1 - ch;
} /* getmenu */
