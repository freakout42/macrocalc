/* lib_stat.c
 * get stat file information
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef WIN32
#include <pwd.h>
#include <grp.h>
#endif
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
#ifndef WIN32
grpbuf	= getgrgid (stbuf.st_gid);
fi.grp	= grpbuf->gr_name;
fi.uid	= stbuf.st_uid;
pwdbuf	= getpwuid (stbuf.st_uid);
fi.own	= pwdbuf->pw_name;
#endif
fi.size	= stbuf.st_size;
fi.time	= localtime (&stbuf.st_mtime);
return &fi;
}
