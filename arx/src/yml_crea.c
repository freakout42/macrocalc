/* $Id: yml_crea.c,v 1.1 2020/06/04 13:13:16 axel Exp $
 * create Yaml-file from dbf-descriptor
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "dbf_def.h"

int yml_crea (struct DBF *d)
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

fprintf (d->ofile_ptr, "# Encoding: ascii-8bit\n\n");
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
	}

/*fprintf (d->ofile_ptr, "LOCK TABLES `%s` WRITE;\n", "dbf_table");*/

return 0;
}
