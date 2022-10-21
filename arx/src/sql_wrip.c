/* $Id: sql_wrip.c,v 1.6 2022/02/09 19:29:52 axel Exp $
 */
#include <stdio.h>
#include <string.h>
#include "arx_def.h"
#include "dbf_def.h"

int sql_wrip (struct DBF *d)
{
unsigned char fld;
struct FIELD_RECORD *f;
char *t;

fprintf (d->ofile_ptr, "INSERT INTO %s (", d->tmpfilename[0]=='\0'?"fromdbf":d->tmpfilename);
for (fld=0; fld<d->fields; fld++)
        {
	f = &d->fields_ptr[fld];
        fprintf (d->ofile_ptr, "`%.*s`", MAX_FIELDNAME, f->name);
	fprintf (d->ofile_ptr, "%s", fld==d->fields-1?"":", ");
        }
fprintf (d->ofile_ptr, ") VALUES (");
for (fld=0; fld<d->fields; fld++)
	{
	f = &d->fields_ptr[fld];
	t = d->record_ptr + f->field_data_address;
	switch (f->typ)
	 {
	 case 'D': fprintf (d->ofile_ptr, "'%4.4s-%2.2s-%2.2s'", t, t+4, t+6); break;
	 case 'N':
	 case 'F': fprintf (d->ofile_ptr, "%s", *t=='\0' ? "null" : t); break;
	 case 'L': fprintf (d->ofile_ptr, "%s", (*t=='\0' ? "null" : ((strchr("TtYyJjOo1", *t)) ? "'t'" : "'f'"))); break;
	 case 'C':
					fputc('\'', d->ofile_ptr);
		while (*t) {
			if (*t == '\'' || *t == '\\' || *t == '%' || *t == '_') fputc('\\', d->ofile_ptr);
					fputc(*t++,   d->ofile_ptr);
		}
					fputc('\'', d->ofile_ptr);
	 }
	fprintf (d->ofile_ptr, "%s", fld==d->fields-1?"":", ");
	}
fprintf (d->ofile_ptr, ");\n");
/*UNLOCK TABLES;*/
return 0;
}
