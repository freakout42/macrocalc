/* $Id: rdb_writ.c,v 1.4 2024/10/30 08:59:20 axel Exp $
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int rdb_writ (struct DBF *d, int m)
{
unsigned char fld;
struct FIELD_RECORD *f;
char *t;

if (m == 1) {
  if (*d->record_ptr == '*') putc ('-', d->ofile_ptr);
  fprintf (d->ofile_ptr, "%d", d->current_record);
  putc (d->fieldsep, d->ofile_ptr);
}
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
