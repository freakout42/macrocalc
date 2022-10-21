/* $Id: sql_crea.c,v 1.2 2020/05/31 11:36:24 axel Exp $
 * create MySQL-table from dbf-descriptor
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "dbf_def.h"

int sql_crea (struct DBF *d)
{
unsigned char fld;
struct FIELD_RECORD *f;
char *s;
struct FIELD_RECORD *fields;

fields = d->fields_ptr;
if ((d->fields_ptr = (struct FIELD_RECORD *)
	malloc ((unsigned)(d->fields * FIELD_REC_LEN)))==NULL)
	return (OUT_OF_MEM);
if ((d->record_ptr = malloc (MAX_RECORD+1))==NULL) return (OUT_OF_MEM);

if (d->todrop == DROPIT)
fprintf (d->ofile_ptr, "DROP TABLE `%s`;\nCREATE TABLE `%s` (\n", d->tmpfilename[0]=='\0'?"fromdbf":d->tmpfilename, d->tmpfilename[0]=='\0'?"fromdbf":d->tmpfilename);
for (fld=1; fld<=d->fields; fld++)
	{
	f = &d->fields_ptr[fld-1];
	*f = *fields++;
	s = f->name;
	switch (d->towhat)
	 {
	 case UPPER:
		str_upr (s);
		break;
	 case LOWER:
		str_low (s);
		break;
	 case CAPITALIZE:
		s[0] = toupper (s[0]);
		str_low (s+1);
		break;
	 case LEAVE:
		break;
	 }
	if (d->todrop == DROPIT) {
	fprintf (d->ofile_ptr, "`%.*s` ", MAX_FIELDNAME, f->name);
	switch (f->typ)
	 {
	 case 'D': fprintf (d->ofile_ptr, "datetime"); break;
	 case 'N': fprintf (d->ofile_ptr, "numeric(%d,%d)", f->len, f->dec); break;
	 case 'F': fprintf (d->ofile_ptr, "numeric(%d,%d)", f->len, f->dec); break;
	 case 'L': fprintf (d->ofile_ptr, "int(1)"); break;
	 case 'C': fprintf (d->ofile_ptr, "char(%d)", f->len); break;
	 case 'M':
	        lib_mesg ("Memo field %.11s not supported", f->name);
	        break;
	 default:
	        lib_fatl (UNKNOWN_TYPE, "Unknown field type");
	 }
	fprintf (d->ofile_ptr, "%s\n", fld==d->fields?"":",");
	}
	}

if (d->todrop == DROPIT)
fprintf (d->ofile_ptr, ");\n");
/*fprintf (d->ofile_ptr, "LOCK TABLES `%s` WRITE;\n", "dbf_table");*/

return 0;
}
