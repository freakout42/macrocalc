/* $Id: lib_dirs.c,v 1.2 2004/04/12 11:16:15 axel Exp $
 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#ifdef __hpux
#include <sys/types.h>
#endif
#include <sys/dir.h>
#include <sys/stat.h>
#include "arx_def.h"

int *lib_diro (char *dirname)
{
struct stat	stbuf;
int		fd;
int		*dp;

if ((fd = open (dirname, O_RDONLY, 0)) == -1
 || fstat (fd, &stbuf) == -1
 || (stbuf.st_mode & S_IFMT) != S_IFDIR
 || (dp = (int *) malloc (sizeof(fd))) == NULL)
	return NULL;
*dp = fd;
return dp;
}

struct direct *lib_dirr (int *dp)
{
static struct direct d;

while (read (*dp, (char *) &d, sizeof(d)) == sizeof(d))
	{
	if (d.d_ino == 0) continue;
	return &d;
	}
return NULL;
}

#ifdef TEST
main (argc, argv)
	int argc;
	char *argv[];
{
int *dp;
struct direct *d;

printf ("%8x\n", dp=lib_diro(argv[1]));
while (d=lib_dirr(dp)) {
	printf ("%s\n", d->d_name);
	}
}
#endif
