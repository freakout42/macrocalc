/* $Id: dbf_writ.c,v 1.2 1996/11/04 09:06:55 axel Exp $
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int dbf_writ (struct DBF *d)
{
return fwrite (d->record_ptr, d->p.record_length, 1, d->file_ptr);
}
