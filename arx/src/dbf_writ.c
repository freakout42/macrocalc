/* dbf_writ.c */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int dbf_writ (struct DBF *d)
{
return fwrite (d->record_ptr, d->p.record_length, 1, d->file_ptr);
}
