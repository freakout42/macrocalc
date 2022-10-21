/* $Id: lib_fsiz.c,v 1.2 1997/04/06 10:53:32 axel Exp $
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arx_def.h>

long lib_fsiz (file)
	int file;
{
int fstat();
struct stat status;

if ((fstat (file, &status)) < 0)
	{
	lib_mesg ("can't get filesize");
	perror ("fsize");
	return RET_ERROR;
	}
return (long) status.st_size;
}
