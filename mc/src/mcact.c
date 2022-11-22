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

int act (char c) {
/* Acts on a particular input */
extern int errpos;
CELLPTR allocated;
cellr cp;
char parsed[MAXPARSED+1];
int col, row, clen, edi;
int first = TRUE;
char s1[MAXINPUT + 1];
char *s;

memset (&cp, 0, sizeof(cellr));
cpcol(&cp) = curcol;
cprow(&cp) = currow;
cptype(&cp) = CONSTANT;
s1[0] = '\0';
s1[1] = c;
s1[2] = '\0';
s = s1+1;
if ((allocated = cell(curcol, currow))) {
  if (c == '\0') {
    strncpy(s1, cptext(allocated), MAXINPUT);
    s1[MAXINPUT] = '\0';
    s = s1;
  } else { return FALSE; }
}
errpos = strlen(s);
do {
	edi	= editstringp(s, "", MAXINPUT, errpos);
	if ((first && !edi) || !*s) { return FALSE; }
	parse(&cp, s, parsed);
	if (cptype(&cp) == SYNERROR) {
		if (!edi) {
			cptype(&cp) = TEXT;
			*--s = STRLEFT;
		}
		else errormsg(MSGSYNTAX);
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
} while (cptype(&cp) == SYNERROR);
cptext(&cp) = s;
allocated = migratcell(allocated, &cp);
if (allocated)
	{
	clen	= celladr(parsed, &col, &row);
	switch(cptype(&cp))
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
