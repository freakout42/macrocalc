#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int errorex(n)
  int n;
{
  fprintf(stderr, "error: %d usage: dbrecall dbf recno\n", n);
  exit(n);
}

int main (argc, argv)
  int argc;
  char *argv[];
{
struct DBFPROLOG2 d2;
struct DBMANPROLOG dm;
struct FIELD_RECORD2 f2[MAX_FIELDS2];
struct FIELD_DBMAN fm[MAX_FIELDS];
unsigned char fld;
unsigned short n;
size_t i = 0;
int recno;
int deleted;
struct DBF dbf;
struct DBF *d;

d = &dbf;

if (argc != 3) errorex(2);
recno = atoi(argv[2]);
if (recno == 0) errorex(19);
strncpy(d->filename, argv[1], sizeof(d->filename));

d->status = not_open;
if ((d->file_ptr = fopen (d->filename,"r+")) == NULL) errorex(1);
i = fread (&d->p.dbf_version, 1, 1, d->file_ptr);
if (i != 1) errorex(3);
switch (d->p.dbf_version & DBVERSION)	/* check for dbase file marker */
 {
 case DBMANFILE: errorex(4);
 case DB2FILE: errorex(5);
 case DB3FILE:
 case DB4FILE:
	i = fread (&d->p.dbf_version+1, sizeof(struct DBFPROLOG)-1, 1, d->file_ptr);
  if (i != 1) errorex(14);
	lib_cano(d->p.records);
  if (recno > d->p.records) errorex(20);
	lib_cano(d->p.header_length);
	lib_cano(d->p.record_length);
	d->fields = ((d->p.header_length - (HEADER_PROLOG+1)) / FIELD_REC_LEN);
	n = HEADER_PROLOG + d->fields * FIELD_REC_LEN + 1;
	if (d->p.header_length == n) d->p.dbf_version |= DB3PLUS;
	else if (d->p.header_length == n+1);
	else errorex(16);
	break;
 default: errorex(6);
 }

if ((d->fields_ptr = (struct FIELD_RECORD *) malloc ((unsigned)(d->fields * FIELD_REC_LEN)))==NULL) errorex(8);
memset (d->fields_ptr, '\0', d->fields * FIELD_REC_LEN);
i = fread (d->fields_ptr, FIELD_REC_LEN, d->fields, d->file_ptr);
if (i != d->fields) errorex(9);

if ((n=fgetc (d->file_ptr)) != DBMARK && (d->p.dbf_version & DBVERSION) != DB2FILE && d->fields < MAX_FIELDS2) errorex(10);
n = d->p.header_length - (HEADER_PROLOG + d->fields*FIELD_REC_LEN + 1);
if (n>0) {
  i = fread (&d2, 1, n, d->file_ptr); /* read rest of header */
  if (i != n) errorex(11);
}
d->current_record = 0L;
if ((d->record_ptr = malloc (d->p.record_length+1))==NULL) errorex(12);

/* initialize pointers to fields in record. */
for (fld=0, n=1; fld<d->fields; fld++)	/* n is offset from start of rec     */
	{				/* @ n=0 is the deleted record flag  */
	d->fields_ptr[fld].field_data_address = n;
	n += d->fields_ptr[fld].len;
	}
if (n != d->p.record_length) errorex(13);

d->status = not_updated;		 /* open successfull */

if (fseek(d->file_ptr, (recno - 1) * d->p.record_length, SEEK_CUR)) errorex(21);
deleted = fgetc(d->file_ptr);
if (deleted != '*') errorex(22);
if (fseek(d->file_ptr, -1, SEEK_CUR)) errorex(23);
deleted = fgetc(d->file_ptr);
if (deleted != '*') errorex(24);
if (fseek(d->file_ptr, -1, SEEK_CUR)) errorex(25);
if ((fputc(' ', d->file_ptr) != ' ')) errorex(30);

fclose(d->file_ptr);
return 0;
}
