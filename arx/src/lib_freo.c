/* $Id: lib_freo.c,v 1.5 1995/05/10 05:04:19 axel Rel $
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <arx_def.h>

FILE *lib_freo (char *filename, char *mode, FILE *stream)
{
int	fd;
FILE	*f;

fd = dup (fileno(stream));
if ( ((f = fdopen (fd, mode)) == NULL)	&&
     ((f = fdopen (fd, "r")) == NULL)	&&
     ((f = fdopen (fd, "w")) == NULL) )	{perror("fdopen");return NULL;}
fcntl (fd, F_SETFD, 1);
if (filename != NULL)
	{
	if (freopen (filename, mode, stream) != stream) return NULL;
	}
else
	fclose (stream);
return f;
}
