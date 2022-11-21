/* mcact.c 1.10 2003/05/25 09:38:30 axel */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcinput.h"
#include "mcmessag.h"
#include "mcparse.h"
#include "mcell.h"
#include "mcellact.h"
#include "mcelladr.h"
#include "mcellpar.h"
#include "mcpipe.h"
#include "mcrecalc.h"
#include "mcget.h"
#include "mcfile.h"
#include "mcdisply.h"
#include "mcact.h"

int act (char *s)
/* Acts on a particular input */
{
extern int	errpos;
int		type;
CELLPTR		allocated;
CELLPTR cp;
char		parsed[MAXPARSED+1];
char		unit[MAXINPUT+1]	= "";
int		col, row, clen, edi;
int		first	= TRUE;

cp = newcell();
origcol	= curcol;
origrow	= currow;
s++;
errpos = strlen(s);
do	{
	edi	= editstringp(s, "", MAXINPUT, errpos);
	if ((first && !edi) || !*s) return FALSE;
	parse(cp, s, parsed);
	if (cptype(cp) == SYNERROR)
		{
		if (!edi)	type = TEXT;
		else		errormsg(MSGSYNTAX);
		}
	switch(errno)
	 {
	 case 0:						break;
	 case ERANGE:		errormsg(MSGERANGE);		break;
	 case EDOM:		errormsg(MSGEDOM);		break;
	 case UNRECUNIT:	errormsg(MSGUNRECUNIT);		break;
	 case CONFUNIT:		errormsg(MSGCONFUNIT);		break;
	 case UNRECRANGE:	errormsg(MSGUNRECRANGE);	break;
	 default:		errorstr(strerror(errno));	break;
	 }
	first	= FALSE;
	} while (type == SYNERROR);
allocated = parsecell(s, curcol, currow);
if (allocated)
	{
	clen	= celladr(parsed, &col, &row);
	switch(type)
	 {
	 case INCOMMAND:
		if (inpipe(col, row, parsed+clen+1) == EOF) allocated = NULL;
		break;
	 case OUTCOMMAND:
		outpipe(col, row, parsed+clen+1);
		break;
	 }
	autorecalc();
	displayscreen(NOUPDATE);
	}
if (!allocated) errormsg(MSGLOMEM);
switch(edi)
 {
 case HOMEKEY:
 case ENDKEY:
 case UPKEY:
 case DOWNKEY:
 case PGUPKEY:
 case PGDNKEY:
 case LEFTKEY:
 case RIGHTKEY:
 case CTRLLEFTKEY:
 case CTRLRIGHTKEY:
	nextkey = edi;
 }
return TRUE;
} /* act */

void getinput (int c)
/* Reads and acts on an input string from the keyboard that started with c. */
{
char s[MAXINPUT + 2];

s[0] = '\0';
s[1] = (char)c;
s[2] = '\0';
changed |= act (s);
} /* getinput */

int checkforsave (void)
/* If the spreadsheet has been changed, will ask the user if they want to
   save it.
*/
{
int	erase = 'N';

while (changed && (erase != 'Y')) {
  if (!getyesno(&erase, MSGSAVESHEET)) return TRUE;
  if (erase == 'N') savesheet();
}
return FALSE;
} /* checkforsave */
