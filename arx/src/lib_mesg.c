/* $Id: lib_mesg.c,v 1.1 1995/04/29 12:37:10 axel Rel $
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "arx_def.h"

void lib_fatl (int s, char *msg, ...)
{
va_list	args;

va_start (args, msg);
#ifdef PROGNAME
fprintf (stderr, "%s: ", progname);
#endif
vfprintf (stderr, msg, args);
fprintf (stderr, "\n");
va_end (args);
exit (s);
}

void lib_mesg (char *msg, ...)
{
va_list	args;

va_start (args, msg);
#ifdef PROGNAME
fprintf (stderr, "%s: ", progname);
#endif
vfprintf (stderr, msg, args);
fprintf (stderr, "\n");
va_end (args);
}
