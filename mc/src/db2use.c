/* $Id: db2use.c,v 1.14 2024/10/30 09:00:43 axel Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include "arx_def.h"
#include "dbf_def.h"

#ifdef MSDOS
#define OPTCHAR	"/"
#else
#define OPTCHAR	"-"
#endif

#ifdef OPTSTR
extern char	opts[];
#else
static char	opts[]	= "options ...";
#endif

void usage(void)
{
fprintf (stderr, "db2rdb - Dbase to /rdb converter - v4.8\n");
fprintf (stderr, "-- (c)2024 Axel K. Reinhold https://mc.freakout.de/ --\n\n");
#ifdef MSDOS
fprintf (stderr, "usage: db2rdb [" OPTCHAR "%s] dbfile [<|> rdbfile]\n\n", opts);
#else
fprintf (stderr, "usage: db2rdb [" OPTCHAR "%s] [file]\n\n", opts);
#endif
/*
fprintf (stderr, OPTCHAR "2   \n");
fprintf (stderr, OPTCHAR "3   \n");
fprintf (stderr, OPTCHAR "4   \n");
fprintf (stderr, OPTCHAR "I   \n");
fprintf (stderr, OPTCHAR "C   \n");
fprintf (stderr, OPTCHAR "D   \n");
fprintf (stderr, OPTCHAR "N   \n");
fprintf (stderr, OPTCHAR "F   \n");
fprintf (stderr, OPTCHAR "L   \n");
*/
fprintf (stderr, "standard options:\n");
fprintf (stderr, OPTCHAR "fc  c is /rdb field-separator\n");
fprintf (stderr, OPTCHAR "i   no data mode\n");
fprintf (stderr, OPTCHAR "K   array mode\n");
fprintf (stderr, OPTCHAR "k   structure mode\n");
fprintf (stderr, OPTCHAR "v   verbose mode\n");
fprintf (stderr, OPTCHAR "o   no drop/create mode\n");
fprintf (stderr, OPTCHAR "u   field name uppercase mode (default in reverse mode)\n");
fprintf (stderr, OPTCHAR "l   field name lowercase mode\n");
fprintf (stderr, OPTCHAR "c   field name capitalize mode - produces clipperfix in B-mode\n");
fprintf (stderr, OPTCHAR "q   MySQL mode\n");
fprintf (stderr, OPTCHAR "z   ODBC mode\n");
fprintf (stderr, OPTCHAR "p   PgSql mode\n");
fprintf (stderr, OPTCHAR "y   Yaml mode\n");
fprintf (stderr, OPTCHAR "A   Advantage illegal mode\n");
fprintf (stderr, OPTCHAR "B   fix Advantage illegal chars\n");
fprintf (stderr, OPTCHAR "mtname SQL table name\n");
fprintf (stderr, OPTCHAR "?   help - print this information\n\n");
fprintf (stderr, "normal options:                 reverse options:\n");
fprintf (stderr, OPTCHAR "b   boolean mode               " OPTCHAR "r       reverse mode\n");
fprintf (stderr, OPTCHAR "d   date mode                  " OPTCHAR "+       plus mode\n");
fprintf (stderr, OPTCHAR "j   recordid mode              " " "     "\n");
fprintf (stderr, OPTCHAR "e   erased mode                " OPTCHAR "0       nullpadding mode\n");
fprintf (stderr, OPTCHAR "g   dash length mode           " OPTCHAR "syymmdd set date mode\n");
fprintf (stderr, OPTCHAR "h   dash headoff mode          " OPTCHAR "afile   append mode to file\n");
fprintf (stderr, OPTCHAR "t   no trim mode               " OPTCHAR "xfile   external file mode\n");

exit (USAGE);
}
