/* $Id: mcrange.c,v 1.2 1995/02/24 09:50:13 axel Rel $
 */

#include <string.h>
#include <stdlib.h>
#include <arx_def.h>
#include "mc.h"
#include "mcrange.h"

int updaterange (char *n, int c1, int r1, int c2, int r2)
{
struct Range	*r;
struct Range	**nrange		= &rnames;

for (r = rnames; r != NULL; r = r->next)
	{
	nrange	= &r->next;
	if (!strcmp (r->name, n)) break;
	}
if (r == NULL)
	{
	if ((r = malloc (sizeof (struct Range))) == NULL ||
	    (r->name = strdup (n)) == NULL) return RET_ERROR;
	*nrange	= r;
	r->next	= NULL;
	}
r->adr[0].col = c1;
r->adr[0].row = r1;
r->adr[1].col = c2;
r->adr[1].row = r2;
return RET_SUCCESS;
}
