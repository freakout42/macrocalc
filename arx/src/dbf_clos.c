/* $Id: dbf_clos.c,v 1.1 1995/04/30 08:47:34 axel Rel $
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int dbf_clos (struct DBF *d)
{
fputc (0x1a, d->file_ptr);
return fclose (d->file_ptr);
}
