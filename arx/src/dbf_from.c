/* dbf_from.c
 * write dbf-structure file to stdout
 */
#include <stdio.h>
#include "arx_def.h"
#include "dbf_def.h"

void dbf_from (struct DBF *d)
{
unsigned char fld;
struct FIELD_RECORD *f;

printf ("FIELD_NAME\tFIELD_TYPE\tFIELD_LEN\tFIELD_DEC\n");
printf ("----------\t-\t---\t---\n");
for (fld=1, f = d->fields_ptr; fld<=d->fields; fld++, f++)
	printf ("%s\t%c\t%d\t%d\n", f->name, f->typ, f->len, f->dec);
}
