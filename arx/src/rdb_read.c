/* $Id: rdb_read.c,v 1.2 2016/11/01 16:20:52 axel Exp $
 * read /rdb-table next record
 */
#include <stdio.h>
#include <string.h>
#include <arx_def.h>
#include <str_def.h>
#include "dbf_def.h"

int rdb_read (struct DBF *d)
{
unsigned char fld;
struct FIELD_RECORD *f;
char *tok, *next;

if (str_gets (d->file_ptr, d->record_ptr, MAX_RECORD) == NULL) return EOF;
next = d->record_ptr;
for (fld=1, f = d->fields_ptr; fld<=d->fields; fld++, f++)
	{
	if (next == NULL)
		{
		lib_mesg ("Too few columns in row %ld", d->current_record);
		break;
		}
	tok = next;
	if ((next = strchr (tok, d->fieldsep)) != NULL) *next++ = '\0';
	f->field_data_address = (d->trim ? str_trim (tok) : tok) - d->record_ptr;
	}
d->current_record++;
return 1;
}
