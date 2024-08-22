/* $Id: rdb_crea.c,v 1.2 2015/07/01 13:31:13 axel Exp $
 * create /rdb-table from dbf-descriptor
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "dbf_def.h"

int rdb_crea (struct DBF *d, int m)
{
unsigned char fld;
int i;
struct FIELD_RECORD *f;
char *s;
struct FIELD_RECORD *fields;

fields = d->fields_ptr;
if ((d->fields_ptr = (struct FIELD_RECORD *)
	malloc ((unsigned)(d->fields * FIELD_REC_LEN)))==NULL)
	return (OUT_OF_MEM);
if ((d->record_ptr = malloc (MAX_RECORD+1))==NULL) return (OUT_OF_MEM);

if (m == 1) {
	fprintf (d->ofile_ptr, "RECNO");
	putc (d->fieldsep, d->ofile_ptr);
}
for (fld=1; fld<=d->fields; fld++)
	{
	f = &d->fields_ptr[fld-1];
	*f = *fields++;
	strncpy (d->record_ptr, f->name, MAX_FIELDNAME);
	d->record_ptr[MAX_FIELDNAME]	= '\0';
	s = d->record_ptr;
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
	fprintf (d->ofile_ptr, "%s", s);
	if (fld<d->fields) putc (d->fieldsep, d->ofile_ptr);
	}
putc ('\n', d->ofile_ptr);
if (d->dashline) {
  if (m == 1) {
    fprintf (d->ofile_ptr, "-----");
    putc (d->fieldsep, d->ofile_ptr);
  }
  for (fld=1, f = d->fields_ptr; fld<=d->fields; fld++, f++) {
    i = d->dashlength ? f->len : min (strlen (f->name), MAX_FIELDNAME);
    if (d->dashlength && f->typ=='D' && d->convdate) i = 8;
    for (; i>0; i--) putc ('-', d->ofile_ptr);
    if (fld<d->fields) putc (d->fieldsep, d->ofile_ptr);
  }
  putc ('\n', d->ofile_ptr);
}
return 0;
}
