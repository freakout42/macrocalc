/* $Id: mcinput.c,v 1.6 1997/05/21 14:59:11 axel Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcstderr.h"
#include "mcgetmsg.h"
#include "mcget.h"
#include "mcmessag.h"
#include "mcelladr.h"
#include "mcgetmnu.h"
#include "mcdisply.h"
#include "mcinput.h"

int getint (int *number, int low, int high, int def)
/* Reads in a positive integer from low to high */
{
int	i, good	= FALSE;
char	n[7], s[MAXSCREENWIDTH+1], message[MAXSCREENWIDTH+1];

getmessage (s, MSGBADNUMBER);
sprintf (message, s, low, high);
sprintf (n, "%d", def);
do
	{
	if (!editstring(n, "1234567890", 6)) return FALSE;
	i = atoi(n);
	if (!(good = (i >= low) && (i <= high))) errorstr (message);
	} while (!good);
*number	= i;
return TRUE;
} /* getint */

int getcell (int *col, int *row)
/* Reads in a cell name that was typed in - Returns FALSE if ESC was pressed */
{
int	first = TRUE, good = FALSE, oldcol = *col, oldrow = *row;
char	input[10] = "";

do
	{
	if (!first) errormsg(MSGBADCELL);
	first = FALSE;
	if (!editstring (input, "", sizeof(input)))
		{
		*col = oldcol;
		*row = oldrow;
		return (FALSE);
		}
	if (celladr (input, col, row)) good = TRUE;
	} while (!good);
return(TRUE);
} /* getcell */

int getyesno (int *yesno, int prompt)
/* Prints a prompt and gets a yes or no answer - returns TRUE if ESC was
   pressed, FALSE if not.
*/
{
int	accept;

writeprompt(prompt);
*yesno	= (accept = getmenu (YNMENU))==1 ? 'Y' : 'N';
return accept==-1 ? FALSE : TRUE;
} /* getyesno */

int generalprompt (void)
{
char	errmsg[MAXSCREENWIDTH+1];
int	key;

if (getstderrmsg(errmsg) != NULL) errorstr(errmsg);

if (menudisp)
	{
	smessage (msgcommand);
	displaymark();
	changeautocalc (autocalc);
	changeformdisplay (formdisplay);
	}
else	clearprompt();

showcelltype();
displaycur (HIGHLIGHT);
clearinput();
if (nextkey==0) return getkey();
else
		{
		key	= nextkey;
		nextkey	= 0;
		return key;
		}
} /* generalprompt */
