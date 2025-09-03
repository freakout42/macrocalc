/* lib_fsiz.c */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arx_def.h>

long lib_fsiz (int file)
{
struct stat status;

if ((fstat (file, &status)) < 0)
	{
	lib_mesg ("can't get filesize");
	perror ("fsize");
	return RET_ERROR;
	}
return (long) status.st_size;
}
