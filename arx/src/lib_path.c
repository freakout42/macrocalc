/* lib_path.c,v 1.6 1997/04/06 10:53:32 axel
 * Path lookup.
 *
 * Path takes a path search list, a file name, and an access mode,
 * and searches each directory in the path list
 * for a file with the given name and access mode.
 * The constructed pathname, which is kept in a static buffer,
 * is returned on success.
 * NULL is returned if either pointer argument is NULL,
 * or if the search failed.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#ifdef MSDOS
#include <access.h>
#include <io.h>
#include "tc_dos.h"
#else
#include <unistd.h>
#endif

char *lib_path (char *path, char *filename, int mode)
{
static char pathname[MAXPATH];
char *dir;
char sep[2] = {LISTSEP, '\0'};
char search[MAXPATH+1];
char *p = search;

if (path == NULL || strlen(path) >= MAXPATH) return NULL;
strcpy (search, path);
while ((dir = strtok (p, sep)))
	{
	strcpy (pathname, dir);
	strcat (pathname, PATHSEPSTRING);
	strcat (pathname, filename);
#ifdef DEBUG
	fprintf (stderr, "access(\"%s\", %4x)\n", pathname, mode);
#endif
	if (!access (pathname, mode)) return pathname;
	p = NULL;
	}
return NULL;
}

#ifdef TEST
main (argc, argv)
	int argc;
	char *argv[];
{
printf ("%s\n", lib_path (getenv("PATH"), argv[1], X_OK));
}
#endif
