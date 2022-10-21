/* $Id: dbf_read.c,v 1.3 1997/04/06 10:51:35 axel Exp $
 * purpose:	routine to get a record from a dbiii file and place in memory at
 *		location pointed to by DBF.record_prt.
 * usage:	d = (struct DBF *)malloc(sizeof(struct DBF));
 *		strcpy(d->filename,"filename.dbf");
 *		dbf_open(d);
 *		dbf_read(d);
 *		... access next record
 *		dbf_close(d);
 *		free(d);
 * returns:	0 if successfull
 *		RECNO_TOO_BIG if record is not in database
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

int dbf_read (struct DBF *d)
{
int	l;
/*int	c;*/
l	= d->p.record_length;
/*if ((d->p.dbf_version & DBVERSION) == DB3FILE &&
    !(d->p.dbf_version & DB3PLUS)) l++; */
d->current_record++;
if (fread (d->record_ptr, l, 1, d->file_ptr) != 1) return 1;
/*why? only for weird systems, crashes dbman files!
 * if ((c = fgetc (d->file_ptr)) != '\0') ungetc (c, d->file_ptr);
 */
return 0;
}
