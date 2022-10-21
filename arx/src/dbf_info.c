/* $Id: dbf_info.c,v 1.7 2020/12/07 11:06:07 axel Exp $
 * write dbf-info to stderr
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

void dbf_info (struct DBF *d)
{
int offsetyr;
unsigned char fld;
struct FIELD_RECORD *f;

fprintf (stderr, "dbase version     ");
if ((d->p.dbf_version & DBVERSION) == DBMANFILE)
	fprintf (stderr, " dBMAN");
else
	{
	fprintf (stderr, "dBASE%d", d->p.dbf_version & DBVERSION);
	if (d->p.dbf_version & DB3PLUS) fputc ('+', stderr);
#ifdef DEBUG
	fprintf (stderr, " dbf_version: \"%2xx\"", d->p.dbf_version);
#endif
	}
fputc ('\n', stderr);
offsetyr = d->p.update_yr >= 70 ? 1900 : 2000;
fprintf (stderr, "update year         %04d\n"  ,d->p.update_yr+offsetyr);
fprintf (stderr, "update month          %02d\n",d->p.update_mo);
fprintf (stderr, "update day            %02d\n",d->p.update_day);
fprintf (stderr, "number of recs %9u\n",	d->p.records);
fprintf (stderr, "header length        %3d\n",	d->p.header_length);
fprintf (stderr, "record length        %3d\n",	d->p.record_length);
fprintf (stderr, "\nFno\tField Name\tType\tLength\tDecimal\n");
for (fld=1, f = d->fields_ptr; fld<=d->fields; fld++, f++)
	fprintf (stderr, "%3u\t%-11.11s\t%c\t%3u\t%3u\n",
		fld, f->name, f->typ, f->len, f->dec);
fputc ('\n', stderr);
}
