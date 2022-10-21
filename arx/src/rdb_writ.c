/* $Id: rdb_writ.c,v 1.2 2016/11/01 16:20:52 axel Exp $
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int rdb_writ (struct DBF *d)
{
unsigned char fld;
struct FIELD_RECORD *f;
char *t;

for (fld=0; fld<d->fields; fld++)
	{
	f = &d->fields_ptr[fld];
	t = d->record_ptr + f->field_data_address;
	fprintf (d->ofile_ptr, "%s", t);
	if ((unsigned char)(fld+1)<d->fields) putc (d->fieldsep, d->ofile_ptr);
	}
putc ('\n', d->ofile_ptr);
return 0;
}
