/* $Id: yml_writ.c,v 1.1 2020/06/04 13:13:16 axel Exp $
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int yml_writ (struct DBF *d)
{
unsigned char fld;
struct FIELD_RECORD *f;
char *t;

fprintf (d->ofile_ptr, "r%07d:\n", d->current_record);
for (fld=0; fld<d->fields; fld++)
        {
	f = &d->fields_ptr[fld];
        fprintf (d->ofile_ptr, "  %.*s: ", MAX_FIELDNAME, f->name);
	t = d->record_ptr + f->field_data_address;
	switch (f->typ)
	 {
	 case 'D': fprintf (d->ofile_ptr, "'%4.4s-%2.2s-%2.2s 00:00:00'", t, t+4, t+6); break;
	 case 'N':
	 case 'F':
	 case 'L': fprintf (d->ofile_ptr, "%s\n", *t=='\0' ? "nil" : t); break;
	 case 'C':
					fputc('\'', d->ofile_ptr);
		while (*t) {
			if (*t == '\'' || *t == '\\') fputc('\\', d->ofile_ptr);
					fputc(*t++, d->ofile_ptr);
		}
					fputc('\'', d->ofile_ptr);
					fputc('\n', d->ofile_ptr);
	 }
	}
fprintf (d->ofile_ptr, "\n");
/*UNLOCK TABLES;*/
return 0;
}
