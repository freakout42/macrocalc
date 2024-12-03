/* lib_freo.c */

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
#ifndef WIN32
fcntl (fd, F_SETFD, 1);
#endif
if (filename != NULL)
	{
	if (freopen (filename, mode, stream) != stream) return NULL;
	}
else
	fclose (stream);
return f;
}
