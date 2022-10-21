/* $Id: rdb_2dbf.c,v 1.4 2020/12/07 09:55:19 axel Exp $
 * copy /rdb-table to dbf_record (structure must be identical!)
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include "dbf_def.h"

int rdb_2dbf (struct DBF *td, struct DBF *sd)
{
unsigned char fld, length;
struct FIELD_RECORD *sf, *tf;
char *tt, *st;
double	val;
int year, month, day;

memset (td->record_ptr, td->nullpad?'\0':' ', td->p.record_length);
for (fld=0; fld<sd->fields; fld++)
	{
	tf = &td->fields_ptr[fld];
	sf = &sd->fields_ptr[fld];
	tt = td->record_ptr + tf->field_data_address;
	st = sd->record_ptr + sf->field_data_address;
	if (st == NULL)
		{
		lib_mesg ("Too few columns in row %ld", sd->current_record);
		break;
		}
	length	= strlen (st);
	if (length != 0) switch (tf->typ)
	 {
	 case 'L':
		if (length == 1 &&
		    strchr("TtYyJjOo1FfNn0 ",*st) != NULL)
		    	*tt = *st;
		else
			{
			lib_mesg ("No valid logical value in field "
				"%.11s record %ld: \"%s\"",
					sf->name, sd->current_record, st);
			*tt = 'F';
			}
		break;
	 case 'D':
		if (length == 6 &&
		    sscanf (st, "%2d%2d%2d", &year, &month, &day) == 3)
			{
			strcpy (tt, "19");
			strncpy (tt+2, st, 6);
			}
		else if (length == 8 &&
			sscanf (st, "%4d%2d%2d", &year, &month, &day) == 3)
			{
			strncpy (tt, st, tf->len);
			}
		else
			{
			lib_mesg ("No valid date value in field "
				 "%.11s record %ld: \"%s\"",
					sf->name, sd->current_record, st);
			strncpy (tt, st, tf->len);
			}
		break;
	 case 'N':
	 case 'F':
		if ((val = str_chkd (st)) != HUGE_VAL)
			{
			sprintf (tt, "%*.*f", tf->len, tf->dec, val);
			if (tt[tf->len] != '\0' ||
			    (tf->dec>0 && tt[tf->len-tf->dec-1]!='.'))
				{
				lib_mesg ("Number value exceeds field width "
					 "%.11s record %ld: \"%s\"",
					    sf->name, sd->current_record, st);
				strcpy (tt, "*");
				}
			}
		else
			{
			lib_mesg ("No valid number value in field "
				 "%.11s record %ld: \"%s\"",
					sf->name, sd->current_record, st);
			strncpy (tt, st, tf->len);
			}
		break;
	 case 'C':
		if (length > tf->len)
			{
			lib_mesg ("field too long in field "
				 "%.11s record %ld: \"%s\"",
					sf->name, sd->current_record, st);
			}
    iso885915_to_oem858(st);
		sprintf (tt, "%-*s", tf->len, st);
		break;
	 case 'X':
		break;
	 default:
		lib_mesg ("unknown field type in field "
			 "%.11s record %ld: \"%s\"",
				sf->name, sd->current_record, st);
		break;
	 }
	}
return 0;
}
