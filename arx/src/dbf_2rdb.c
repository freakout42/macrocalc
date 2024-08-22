/* $Id: dbf_2rdb.c,v 1.7 2020/12/07 09:55:19 axel Exp $
 * copy dbf_record to /rdb-table (structure must be identical!)
 *
 * these cannot be found by ADSlocal driver:
 *   [158, 169, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
 *    199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 222, 223, 224,
 *    226, 227, 228, 229, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 242, 243, 244, 245, 247, 249, 251, 252, 254]
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <str_def.h>
#include "arx_def.h"
#include "dbf_def.h"

int dbf_2rdb (struct DBF *td, struct DBF *sd)
{
unsigned char fld;
struct FIELD_RECORD *sf, *tf;
char *tt, *st, *s, *t;
char buff[MAX_FIELDSIZE+1];
int status;
char adsillegals[] = "\x9e\xa9\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xde\xdf\xe0\xe2\xe3\xe4\xe5\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf2\xf3\xf4\xf5\xf7\xf9\xfb\xfc\xfe";

status = 0;
tt = td->record_ptr;
*tt++ = *sd->record_ptr;
*tt++ = '\0';
for (fld=0; fld<sd->fields; fld++)
	{
	tf = &td->fields_ptr[fld];
	sf = &sd->fields_ptr[fld];
	st = sd->record_ptr + sf->field_data_address;
	td->current_record = sd->current_record;
	memcpy (buff, st, sf->len);
	buff[sf->len] = '\0';
	s = buff;
	if (td->trim) s = str_trim (s);
	while ((t = strchr (s, td->fieldsep)))
		{
		*t = ' ';
		lib_mesg
		 ("fieldseparator-char in field %.11s record %ld: "
		  "\"%s\"\n\treplaced by space",
		  sf->name, sd->current_record, s);
		}
	switch (tf->typ)
	 {
	 case 'D':
		if (td->convdate) s += 2;
		goto putfield;
	 case 'N':
	 case 'F':
		goto putfield;
	 case 'L':
		if (td->bool)
			{
			if (strchr ("TtYyJjOo1", *s) != NULL)
				*s = '1';
			else	*s = '0';
			}
		goto putfield;
	 case 'C':
    oem858_to_iso885915(s);
		if (strpbrk(s, adsillegals)) status = 1;
		goto putfield;
	 case 'M':
		break;
	 putfield:
		strcpy (tt, s);
		tf->field_data_address	= tt - td->record_ptr;
		tt	+= strlen(s)+1;
	 }
	}
return status;
}
