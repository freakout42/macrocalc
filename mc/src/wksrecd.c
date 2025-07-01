/* $Id: wksrecd.c,v 1.4 1995/03/09 19:40:47 axel Rel $
 */

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <arx_def.h>
#include "lotus.h"
#include "lics.h"
#include "mc2wks.h"

int getwksrecord (Lotrec *r, FILE *fil)
{
int	n;

n = fread (r, sizeof(short), 2, fil);
lib_cano (r->opcode);
lib_cano (r->reclen);
#ifdef DEBUG
fprintf (stderr, "getr: read=%d oc=%d l=%d\n", n, r->opcode, r->reclen);
#endif
if (n != 2) fatal (11, "unexpected EOF reached, file shortend.");
if (r->reclen)
	{
	n = fread (&r->data, r->reclen, 1, fil);
	if (n != 1) fatal (11, "unexpected EOF reached, file shortend.");
	}
return r->opcode;
}

int putwksrecord (Lotrec *r, FILE *fil)
{
int rl;
#ifdef DEBUG
fprintf (stderr, "putr: oc=%d l=%d\n", r->opcode, r->reclen);
#endif
rl = r->reclen;
lib_cano (r->opcode);
lib_cano (r->reclen);
fwrite (r, sizeof(short), 2, fil);
return fwrite (&r->data, rl, 1, fil);
}
