/* $Id: dbf_crea.c,v 1.9 2016/11/01 16:20:52 axel Exp $
 * create dbf-file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arx_def.h"
#include "dbf_def.h"

int dbf_crea (struct DBF *d)
{
unsigned char fld;
unsigned short n;
struct FIELD_RECORD *fields;

d->p.dbf_version = DB3FILE;			/* dbase III file marker */
n = HEADER_PROLOG + d->fields*FIELD_REC_LEN + 1;/* plus header length */
if (d->p.header_length != n && d->p.header_length != n+1)
	d->p.header_length = n;
#ifdef NOPLUS
if (d->p.header_length == n)
	d->p.dbf_version |= DB3PLUS;		/* dbase III plus file marker */
#endif

fields = d->fields_ptr;
if ((d->fields_ptr = (struct FIELD_RECORD *)
	malloc ((unsigned)(d->fields * FIELD_REC_LEN)))==NULL)
	return (OUT_OF_MEM);
if ((d->record_ptr = malloc (d->p.record_length+1))==NULL)
	return (OUT_OF_MEM);

/* initialize fields in record. */
for (fld=0, n=1; fld<d->fields; fld++)	/* n is offset from start of rec     */
	{				/* @ n=0 is the deleted record flag  */
	d->fields_ptr[fld] = *fields++;
	n += d->fields_ptr[fld].len;
	d->fields_ptr[fld].field_data_address = 0;
	memset (&d->fields_ptr[fld].reserved_bytes[0], '\0',
		sizeof(d->fields_ptr[fld].reserved_bytes));
	}

if (n != d->p.record_length)
	{
	lib_mesg ("warning: record length wrong\n");
	d->p.record_length = n;
/*	return (BAD_FORMAT);*/
	}

lib_cano(d->p.records);
lib_cano(d->p.header_length);
lib_cano(d->p.record_length);
if (fwrite (&d->p, HEADER_PROLOG, 1, d->file_ptr) != 1) return (IO_ERROR);
if (fwrite (d->fields_ptr, FIELD_REC_LEN, d->fields, d->file_ptr) != d->fields)
	return (IO_ERROR);
fputc (DBMARK, d->file_ptr);
lib_cano(d->p.records);
lib_cano(d->p.header_length);
lib_cano(d->p.record_length);
if (d->p.header_length - 1 == HEADER_PROLOG + FIELD_REC_LEN*d->fields + 1)
	fputc ('\0', d->file_ptr);

/* initialize pointers to fields in record. */
for (fld=0, n=1; fld<d->fields; fld++)	/* n is offset from start of rec     */
	{				/* @ n=0 is the deleted record flag  */
	d->fields_ptr[fld].field_data_address = n;
	n += d->fields_ptr[fld].len;
	}

d->status = not_updated;		 /* open successfull */
return 0;
}
