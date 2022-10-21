/* $Id: mcommand.c,v 1.11 2001/06/06 14:30:37 axel Exp $
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arx_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcmessag.h"
#include "mcinput.h"
#include "mcget.h"
#include "mcdisply.h"
#include "mcwindow.h"
#include "mcellact.h"
#include "mcrange.h"
#include "mcrwalk.h"
#include "mcact.h"
#include "mcrecalc.h"
#include "mcommand.h"
#include "mcfileio.h"
#include "mcellval.h"

void newpage (void)
/* Adds new page sign */
{
alloctext (curcol, currow, ".bp");
changed = TRUE;
} /* newpage */

void editcell (CELLPTR ecell)
/* Edits a selected cell */
{
char s[MAXINPUT + 2];

if (ecell == NULL) return;
strcpy (s+1, ecell->text);
changed |= act (s);
} /* editcell */

void setcolwidth (int col)
/* Sets the new column width for a selected column */
{
int width;

writeprompt (MSGCOLWIDTH);
if (!getint(&width, MINCOLWIDTH, MAXCOLWIDTH, defaultwidth)) return;
colwidth[col] = (unsigned char) width;
setrightcol();
if (rightcol < col)
	{
	rightcol = col;
	setleftcol();
	setrightcol();
	}
displayscreen (NOUPDATE);
changed = TRUE;
} /* setcolwidth */

void gotocell (void)
/* Moves to a selected cell */
{
writeprompt(MSGGOTO);
if (!getcell(&curcol, &currow)) return;
leftcol = curcol;
windowrow = currow;
setrightcol();
setleftcol();
setbottomrow();
displayscreen(NOUPDATE);
} /* gotocell */

static unsigned int andformat;
static unsigned int orformat;

static int formcell (int col, int row)
{
CELLPTR	cp = cell (col, row);

cp->format &= andformat;
cp->format |= orformat;
displaycell (col, row, NOHIGHLIGHT, NOUPDATE);
return RET_SUCCESS;
} /* formcell */

static int attribcell (int col, int row)
{
CELLPTR	cp = cell (col, row);

cp->attrib &= TYPEM;
if (orformat)	cp->attrib |= orformat;
else		cp->attrib &= ~(BOLD | ITALIC);
displaycell (col, row, NOHIGHLIGHT, NOUPDATE);
return RET_SUCCESS;
} /* attribcell */

void formatcells (int and, int or)
/* Prompts the user for a selected format and range of cells */
{
int		temp;
struct CELLADR	start, end;

andformat = and;
orformat  = or;
if (!(and & FORMATM)) switch (or & FORMATM)
 {
 case FIXED:
 case SCIENTIFIC:
 case COMMA:
 case PERCENT:
	writeprompt(MSGPLACES);
	if (!getint(&temp, 0, MAXPLACES, (defaultformat&FORMATM)!=SPECIAL ? defaultformat&PLACES : 2)) return;
	orformat |= temp;
 }
start.col = omarkcol;
start.row = omarkrow;
end.col   = markcol;
end.row   = markrow;
rangewalk (formcell, start, end);
displayscreen(NOUPDATE);
changed = TRUE;
} /* formatcells */

void attribcells (int att)
/* Sets cell attributes */
{
struct CELLADR	start, end;

orformat  = att;
start.col = omarkcol;
start.row = omarkrow;
end.col   = markcol;
end.row   = markrow;
rangewalk (attribcell, start, end);
displayscreen(NOUPDATE);
changed = TRUE;
} /* attribcells */

void deletecells (void)
/* deletes a range of cells */
{
struct CELLADR start, end;

start.col = omarkcol;
start.row = omarkrow;
end.col   = markcol;
end.row   = markrow;
rangewalk (deletecell, start, end);
changed = TRUE;
if (autocalc) recalc();
displayscreen(NOUPDATE);
}

void valuecells (void)
/* values a range of cells */
{
struct CELLADR start, end;

start.col = omarkcol;
start.row = omarkrow;
end.col   = markcol;
end.row   = markrow;
rangewalk (valuecell, start, end);
changed = TRUE;
if (autocalc) recalc();
displayscreen(NOUPDATE);
}

void createrange (void)
/* Creates a named range */
{
char rangename[MAXFILE+1]	= "";

writeprompt(MSGRANGENAME);
if (!editstring (rangename, "", MAXINPUT)) return;
if (strlen (rangename) <= 2 || !isalpha (*rangename)) {
	errormsg (MSGUNRANGE);
	return;
	}
if (updaterange (rangename, omarkcol, omarkrow, markcol, markrow))
	{
	errormsg (MSGLOMEM);
	return;
	}
changed	= TRUE;
} /* createrange */

void deleterange (void)
/* Deletes a named range */
{
char rangename[MAXFILE+1]	= "";
struct Range *r;

writeprompt(MSGRANGENAME);
if (!editstring (rangename, "", MAXINPUT)) return;
for (r = rnames; r != NULL; r = r->next)
	if (strcmp (r->name, rangename))
		{
		free (r->name);
		r->name	= NULL;
		}
changed	= TRUE;
} /* deleterange */

void insdelcolrow (int mode)
/* Insert/Delete Column(s)/Row(s) */
{
int n;

writeprompt (MSGINSDEL);
if (!getint(&n, 1, MAXROWS, 1)) return;
for (; n; n--) switch (mode)
 {
 case INSCOL:	insertcol(curcol);
		break;
 case DELCOL:	deletecol(curcol);
		break;
 case INSROW:	insertrow(currow);
		break;
 case DELROW:	deleterow(currow);
		break;
 }
displayscreen (NOUPDATE);
changed = TRUE;
} /* insdelcolrow */
