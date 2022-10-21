/* $Id: rdb_open.c,v 1.8 2020/12/07 11:06:07 axel Exp $
 * purpose:	routine to open a /rdb file for access by the
 *		other routines
 * usage:	d = (struct DBF *)malloc(sizeof(struct DBF));
 *		strcpy(d->filename,"filename.dbf");
 *		d_open(d);
 *		... access file with other routines ...
 *		d_close(d);
 *		free(d);
 *		d->status == not_updated ... use fast mode, only one read
 * returns:	0	if successful with structure filled in
 *		NO_FILE	 if unable to find file
 *		NO_TMP_FILE if unable to write tmp file
 *		OUT_OF_MEM if not enough memory
 *		BAD_FORMAT if not /rdb file
 */

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "dbf_def.h"

int rdb_open (struct DBF *d)
{
unsigned char fld;
short n;
int row, col;
FILE *rdbfile = NULL;
char *t, *tok, *next;
char *p;
struct FIELD_RECORD *f;
unsigned int len, prec;
int year;
int month;
int day;
char rdbname[]="/tmp/rdbopXXXXXX";
int temp_fd;

if ((d->record_ptr = malloc (MAX_RECORD+1))==NULL) return(OUT_OF_MEM);

if (d->file_ptr == NULL)
	{
	if ((d->file_ptr = fopen (d->filename,"r")) == NULL) return (NO_FILE);
	*d->tmpfilename = '\0';
	}
else
	{
	if (d->status!=not_updated)
		{
		temp_fd=mkstemp(rdbname);
		if ((rdbfile = fdopen (temp_fd, "w+")) == NULL)
			return (NO_TMP_FILE);
		unlink (rdbname);
		strcpy (d->tmpfilename, rdbname);
		}
	}

for (row=0; str_gets (d->file_ptr, d->record_ptr, MAX_RECORD); row++)
	{
#ifdef DEBUG
	fprintf (stderr, "%s\n", d->record_ptr);
#endif
	if (rdbfile != NULL) fprintf (rdbfile, "%s\n", d->record_ptr);
	if (row==0)
		{
		next	= d->record_ptr;
		for (col = 0; next != NULL; col++)
			{
			t	= next;
			next	= strchr (t, d->fieldsep);
			if (next!=NULL) next++;
			}
		d->fields = (unsigned char)col;
		if ((d->fields_ptr = (struct FIELD_RECORD *)
			malloc ((unsigned)(d->fields * FIELD_REC_LEN)))==NULL)
			return (OUT_OF_MEM);
		memset (d->fields_ptr, '\0', d->fields * FIELD_REC_LEN);
		}
	next	= d->record_ptr;
	for (col = 0; next != NULL; col++)
		{
		f	= &d->fields_ptr[col];
		tok	= next;
		if ((next = strchr (tok, d->fieldsep)) != NULL) *next++ = '\0';
		t	= str_trim (tok);
		switch (row)
			 {
			 case 0:
				if (*t == '\0') return (BAD_FORMAT);
				switch (d->towhat)
				 {
				 case UPPER:
					str_upr (t);
					break;
				 case LOWER:
					str_low (t);
					break;
				 case CAPITALIZE:
					t[0]	= toupper (t[0]);
					str_low (t+1);
					break;
				 case LEAVE:
					break;
				 }
				strncpy (f->name, t, MAX_FIELDNAME-1);
				if ((int)strlen(t) > MAX_FIELDNAME-1)
					lib_mesg
					("long fieldname %s truncated to %.*s",
						t, MAX_FIELDNAME-1, t);
				f->typ = 'C';
				f->len = 8;
				f->dec = 0;
				f->maxlen = 0;
				break;
			 case 1:
				if (*t == '\0' && d->dashline) return (BAD_FORMAT);
				if (!d->dashline) goto no_dashline;
				f->len = strlen(t);
				f->maxlen = f->len;
				if (strspn (t, "-") != f->len)
					{
					return (BAD_FORMAT);
					}
			 	break;
			 case 2:
			 no_dashline:
				f->typ = f->maxlen>1 ? 'D' : 'L';
				f->len = f->maxlen>1 ? 8   : 1;
				f->dec = 0;
				/*FALLTHRU*/
			 default:
				if (*t == '\0') break;
				len = strlen(t);
				f->maxlen = (unsigned char) max(len, f->maxlen);
				switch (f->typ)
				 {
				 case 'L':
					if (len == 1 &&
					    strcmp(f->name, "FIELD_TYPE") &&
					    strchr("TtYyJjOo1FfNn0",*t) != NULL)
						break;
					f->typ	= 'D';
					f->len	= 8;
					/*FALLTHRU*/
				 case 'D':
					if (len == 6 &&
					    sscanf (t, "%2d%2d%2d",
					     &year, &month, &day) == 3 &&
					    month>0 && month<=12 &&
					    day>0 && day<=31)
						break;
					else if (len == 8 &&
					    sscanf (t, "%4d%2d%2d",
					     &year, &month, &day) == 3 &&
					    month>0 && month<=12 &&
					    day>0 && day<=31)
						break;
					f->typ	= 'N';
					f->len	= 1;
					/*FALLTHRU*/
				 case 'N':
				 case 'F':
					if (str_chkd(t) != HUGE_VAL)
						{
						if ((p = strchr(t,'.')) == NULL)
							prec = 0;
						else
							prec = len-(p-t)-1;
						f->len = max(f->len,f->maxlen);
						f->dec = (unsigned char)
							max(f->dec, prec);
						if (f->len>MAX_NFIELD)
							f->typ = 'F';
						break;
						}
					f->typ	= 'C';
					f->dec	= 0;
					}
					/*FALLTHRU*/
				 case 'C':
					f->len	= max (f->len, f->maxlen);
					break;
			 }
#ifdef DEBUG
	fprintf (stderr, ":%s:%c%d%d\n", t, f->typ, f->len, f->dec);
#endif
		}
	if (d->status==not_updated && row==1) break;
#ifdef DEBUG
	dbf_info(d);
#endif
	}
if (row==0) return (BAD_FORMAT);

if (d->status!=not_updated)
	{
	if (rdbfile != NULL)
		{
		fclose (d->file_ptr);
		d->file_ptr	= rdbfile;
		}
	rewind (d->file_ptr);
	str_gets (d->file_ptr, d->record_ptr, MAX_RECORD);
	str_gets (d->file_ptr, d->record_ptr, MAX_RECORD);
	}

d->status		= not_open;
d->p.dbf_version	= DB3FILE
#ifdef NOPLUS
 | DB3PLUS
#endif
;
d->current_record	= 0L;
d->p.records		= row -= 2;
d->p.header_length	= HEADER_PROLOG + d->fields * FIELD_REC_LEN + 1;
d->p.record_length	= 1;

for (fld=0; fld<d->fields; fld++)
	{
	d->p.record_length += d->fields_ptr[fld].len;
	}
for (fld=0, n=1; fld<d->fields; fld++)
	{
	d->fields_ptr[fld].field_data_address = n;
	n += d->fields_ptr[fld].len;
	}
d->status = not_updated;		 /* open successfull */
#ifdef DEBUG
dbf_info(d);
#endif
return 0;
}

