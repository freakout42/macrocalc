/* $Id: rdb_clos.c,v 1.1 1995/04/30 08:02:25 axel Rel $
 */
#include <stdio.h>
#include <unistd.h>
#include "arx_def.h"
#include "dbf_def.h"

int rdb_clos (struct DBF *d)
{
if (d->file_ptr != NULL) fclose (d->file_ptr);
if (d->ofile_ptr != NULL) fclose (d->ofile_ptr);
if (d->tmpfilename[0] != '\0') unlink (d->tmpfilename);
return 0;
}
