/* $Id: mcrmacro.c,v 1.5 2002/10/17 12:25:50 axel Exp $
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcget.h"
#include "mcmessag.h"
#include "mcrmacro.h"

void runmacro (int prompt)
/* Runs a macro */
{
char	macrocall[MAXINPUT+1] = "";

if (prompt)
	{
	writeprompt(MSGMACRONAME);
	if (!editstring (macroname, "", MAXINPUT)) return;
	}
if (*macroexec) sprintf (macrocall, "echo '%s'|%s/bin/mcmacro", macroexec, libpath);
else if (!*macroname || !strcmp (macroname, "-")) return;
else	{
	if (access(macroname, R_OK))
		{
		errormsg(MSGNOEXIST);
		return;
		}
	sprintf (macrocall, "%s/bin/mcmacro <%s", libpath, macroname);
	}
if (cur_maco (macrocall) == NULL)
	{
	errormsg(MSGNOOPEN);
	return;
	}
} /* runmacro */
