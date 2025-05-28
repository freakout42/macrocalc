/* $Id: mcprint.c,v 1.6 2001/06/06 06:01:16 axel Exp $
 */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <arx_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcellact.h"
#include "mcellstr.h"
#include "mcelladr.h"
#include "mcutil.h"
#include "mcfileio.h"
#include "mcprint.h"

void printfile (FILE *file, int border)
/* Prints a copy of the spreadsheet to a file */
{
char	s[MAXPRINT+1], cs[MAXPRINT+1], colstr[MAXPRINT+1];
int	col, row, dummy;

if (border)
	{
	snprintf (s, MAXPRINT, "%*s\\fB", LEFTMARGIN, "");
	for (col = omarkcol; col <= markcol; col++)
		{
		centercolstring (col, colstr);
		strcat (s, colstr);
		}
	fprintf (file, "%s\\fP\n", s);
	}
for (row = omarkrow; row <= markrow; row++)
	{
	if (border) snprintf (s, MAXPRINT, "\\fB%*d\\fP", LEFTMARGIN, row + 1);
	else	s[0] = '\0';
	for (col = omarkcol; col <= markcol; col++)
		strcat (s, cellstring (col, row, &dummy, FPRINT));
	strcpy (cs, s);
	if (strtok(cs, " ") == NULL) s[0] = '\0';
	fprintf (file, "%s\n", s);
	}
} /* printfile */

void csvfile (FILE *file)
/* Dumps a copy of the spreadsheet to a csv-file */
{
int	col, row, dummy;

for (row = omarkrow; row <= markrow; row++)
	{
	for (col = omarkcol; col <= markcol; col++) {
		fprintf (file, "\"%s\"", cellstring (col, row, &dummy, NOCOLUMN));
		if (col < markcol) fputc(',', file);
		}
	fputc('\n', file);
	}
} /* csvfile */
