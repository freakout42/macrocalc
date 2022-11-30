/* mcellpar.c 1.5 2001/11/06 15:58:30 axel */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arx_def.h>
#include "mc.h"
#include "mcellstr.h"
#include "mcparse.h"
#include "mcell.h"
#include "mcelladr.h"

#ifdef PARSECELLOBSOLETED
CELLPTR parsecell (char *strvalue, int col, int row)
/* Parses a cell */
{
assert(FALSE);
#ifdef NOMORENEEDED
int	pstatus;
unsigned char format		= defaultformat;
char	vbuf[MAXINPUT+1]	= "";
char	parsed[MAXPARSED+1];
CELLPTR	cp, allocated;

origcol	= col;
origrow	= row;
strcpy(s, strvalue);
cp = newcell();
cptext(cp) = vbuf+1;
pstatus = parse (cp, s, parsed);
#ifdef DEBUG
fprintf (stderr, "par: %s->%e type:%d unit=%s errno:%d\n", s, cpvalue(cp), cptype(cp), cpunit(cp), pstatus);
#endif

switch (cptype(cp)) {
 case DATETYPE:
	cptype(cp) = CONSTANT;
	format = SPECIAL|DATE;
	break;
 case SYNERROR:
	cptype(cp) = TEXT;
	/*FALLTHRU*/
 case TEXT:
	switch (*s) {
	 case STRLEFT:
	 case STRRIGHT:
	 case STRCENTER:
	 case STRREPEAT:
		break;
	 default:
		*--s = STRLEFT;
	 }
	break;
 }
cptext(cp) = s;
allocated = cell(col, row);
allocated = migratcell(allocated, cp);
#ifdef DEBUG
fprintf (stderr, "inited: type:%d\n", cptype(allocated));
#endif
return allocated;
#endif
} /* parsecell */
#endif
