/* dbf_open.c 1.6 2016/11/01 16:20:52 axel
 * purpose:		routine to open a dbaseiii file for access by the
 *			other routines in dbf.lib.
 * usage:		d = (struct DBF *)malloc(sizeof(struct DBF));
 *			strcpy(d->filename,"filename.dbf");
 *			dbf_open(d);
 *			... access file with other routines ...
 *			dbf_close(d);
 *			free(d);
 * notes:		see dbf_def.h for structure of DBF.
 *			copy filename into structure before calling dbf_open
 * returns:		0 if successful with structure filled in
 *			NO_FILE  if unable to find file
 *			OUT_OF_MEM if not enough memory
 *			BAD_FORMAT if not dBASE file
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"
#include "str_def.h"

int dbf_open (struct DBF *d)
{
struct DBFPROLOG2	d2;
struct DBMANPROLOG	dm;
struct FIELD_RECORD2	f2[MAX_FIELDS2];
struct FIELD_DBMAN	fm[MAX_FIELDS];
unsigned char fld;
unsigned short n;
size_t i = 0;

d->status = not_open;
if (d->file_ptr == NULL)
	if ((d->file_ptr = fopen (d->filename,"r+b")) == NULL &&
	    (d->file_ptr = fopen (d->filename,"rb")) == NULL)
		return (NO_FILE);

i = fread (&d->p.dbf_version, 1, 1, d->file_ptr);
if (i != 1) goto badformat;
#ifdef DEBUG
fprintf (stderr, "version: %2x %d\n",
		d->p.dbf_version, d->p.dbf_version & DBVERSION);
#endif
switch (d->p.dbf_version & DBVERSION)	/* check for dbase file marker */
 {
 case DBMANFILE:
	dm.v.dbman_ver[0] = d->p.dbf_version;
	i = fread (&dm.v.dbman_ver[1], sizeof(struct DBMANPROLOG)-1, 1, d->file_ptr);
  if (i != 1) goto badformat;
	lib_cano(dm.v.dbman_version);
	lib_cano(dm.records);
	lib_cano(dm.record_length);
	lib_cano(dm.ddesc_length);
	lib_cano(dm.header_length);
	lib_cano(dm.field_count);
	if (dm.v.dbman_version!=DBMAN)
		{
		fprintf (stderr, "unknown DBMAN V version %4x\n",
			 dm.v.dbman_version);
		goto badformat;
		}
	d->p.update_yr		= dm.update_yr;
	d->p.update_mo		= dm.update_mo;
	d->p.update_day		= dm.update_day;
	d->p.records		= dm.records;
	d->p.record_length	= dm.record_length+1;
	d->p.header_length	= dm.header_length;
	d->fields		= (unsigned char)dm.field_count;
	if (dm.field_count * sizeof(struct FIELD_DBMAN) !=
	    dm.ddesc_length - sizeof(dm.field_count) ||
	    dm.header_length != dm.ddesc_length + DBMAN_HDLG)
	    	{
		fprintf (stderr, "DBMAN V description length error\n");
		goto badformat;
		}
	/* read into field description array DBMAN */
	i = fread (fm, sizeof (struct FIELD_DBMAN), d->fields, d->file_ptr);
  if (i != d->fields) goto badformat;
	for (fld=0; fld<d->fields; fld++) lib_cano(fm[fld].len);
	break;
 case DB2FILE:
	i = fread (&d2.dbf_version+1, sizeof(struct DBFPROLOG2)-1, 1, d->file_ptr);
  if (i != 1) goto badformat;
	lib_cano(d2.records);
	lib_cano(d2.record_length);
	d->p.update_yr		= d2.update_yr;
	d->p.update_mo		= d2.update_mo;
	d->p.update_day		= d2.update_day;
	d->p.records		= d2.records;
	d->p.record_length	= d2.record_length;
	/* read into field description array II */
	i = fread (f2, sizeof (struct FIELD_RECORD2), MAX_FIELDS2, d->file_ptr);
  if (i != MAX_FIELDS2) goto badformat;
	for (fld=0; fld<MAX_FIELDS2; fld++)
		{
		lib_cano(f2[fld].field_data_address);
		if (*f2[fld].name == DBMARK) break;
		}
	d->fields = fld;
	break;
 case DB3FILE:
 case DB4FILE:
	i = fread (&d->p.dbf_version+1, sizeof(struct DBFPROLOG)-1, 1, d->file_ptr);
  if (i != 1) goto badformat;
	lib_cano(d->p.records);
	lib_cano(d->p.header_length);
	lib_cano(d->p.record_length);
	d->fields = ((d->p.header_length - (HEADER_PROLOG+1)) / FIELD_REC_LEN);
	n = HEADER_PROLOG + d->fields * FIELD_REC_LEN + 1;
	if (d->p.header_length == n) d->p.dbf_version |= DB3PLUS;
	else if (d->p.header_length == n+1);
	else
		{
		fprintf (stderr, "dBASE III header_length %d wrong\n",
			 d->p.header_length);
		goto badformat;
		}
	break;
 default:
	fprintf (stderr,
		 "unknown version indicator %d\n", d->p.dbf_version&DBVERSION);
		goto badformat;
 }

if ((d->fields_ptr = (struct FIELD_RECORD *)
	malloc ((unsigned)(d->fields * FIELD_REC_LEN)))==NULL)
	return (OUT_OF_MEM);
memset (d->fields_ptr, '\0', d->fields * FIELD_REC_LEN);

switch (d->p.dbf_version & DBVERSION)	/* read into field description array III */
 {
 case DBMANFILE:
	for (fld=0; fld<d->fields; fld++)
		{
		strncpy_t (d->fields_ptr[fld].name, fm[fld].name, MAX_FIELDNAME);
		d->fields_ptr[fld].typ	= fm[fld].typ;
		d->fields_ptr[fld].len	= fm[fld].len;
		d->fields_ptr[fld].dec	= fm[fld].dec;
		}
	break;
 case DB2FILE:
	for (fld=0; fld<d->fields; fld++)
		{
		strncpy_t (d->fields_ptr[fld].name, f2[fld].name, MAX_FIELDNAME);
		d->fields_ptr[fld].typ	= f2[fld].typ;
		d->fields_ptr[fld].len	= f2[fld].len;
		d->fields_ptr[fld].dec	= f2[fld].dec;
		}
	break;
 case DB3FILE:
 case DB4FILE:
	i = fread (d->fields_ptr, FIELD_REC_LEN, d->fields, d->file_ptr);
  if (i != d->fields) goto badformat;
	break;
 }

if ((d->p.dbf_version & DBVERSION) != DBMANFILE)
	{
	if ((n=fgetc (d->file_ptr)) != DBMARK &&
	   (d->p.dbf_version & DBVERSION) != DB2FILE && d->fields < MAX_FIELDS2)
		{
		fprintf (stderr, "tailer wrong\n");
		return (BAD_FORMAT);
		}
	n = d->p.header_length - (HEADER_PROLOG + d->fields*FIELD_REC_LEN + 1);
	if ((d->p.dbf_version & DBVERSION)==DB3FILE &&
	    !(d->p.dbf_version&DB3PLUS) && n != 1)
		lib_mesg ("warning: plus tailer wrong\n");
	if (n>0) {
    i = fread (&d2, 1, n, d->file_ptr); /* read rest of header */
    if (i != n) goto badformat;
    }
	}

d->current_record = 0L;

if ((d->record_ptr = malloc (d->p.record_length+1))==NULL)
	return(OUT_OF_MEM);

/* initialize pointers to fields in record. */
for (fld=0, n=1; fld<d->fields; fld++)	/* n is offset from start of rec     */
	{				/* @ n=0 is the deleted record flag  */
	d->fields_ptr[fld].field_data_address = n;
	n += d->fields_ptr[fld].len;
	}

if (n != d->p.record_length)
	{
	lib_mesg ("warning: record length wrong");
	d->p.record_length = n;
/*	return (BAD_FORMAT);*/
	}

d->status = not_updated;		 /* open successfull */
return (0);

badformat:
	fclose (d->file_ptr);
	fprintf (stderr, "header wrong i=%d\n", (int)i);
	return (BAD_FORMAT);
}
