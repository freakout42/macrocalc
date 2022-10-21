/* $Id: lib_stat.c,v 1.5 2004/04/12 11:16:15 axel Exp $
 * get stat file information
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#ifdef __hpux
#include <sys/types.h>
#endif
#include <sys/stat.h>
#include "arx_def.h"

struct file *lib_stat (char *f)
{
static struct file	fi;
struct stat		stbuf;
struct passwd		*pwdbuf;
struct group		*grpbuf;
static char		basename[DIRSIZ+1];
char			*fn;

if (stat (f, &stbuf) == -1) return NULL;
fn	= strrchr (f, PATHSEP);
strncpy (basename, fn ? fn+1 : f, DIRSIZ+1);
fi.name	= basename;
fi.mode	= stbuf.st_mode;
fi.gid	= stbuf.st_gid;
grpbuf	= getgrgid (stbuf.st_gid);
fi.grp	= grpbuf->gr_name;
fi.uid	= stbuf.st_uid;
pwdbuf	= getpwuid (stbuf.st_uid);
fi.own	= pwdbuf->pw_name;
fi.size	= stbuf.st_size;
fi.time	= localtime (&stbuf.st_mtime);
return &fi;
}
