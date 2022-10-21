/* $Id: ibm2lics.c,v 1.1 1995/03/06 19:28:09 axel Rel $
 */

#include <stdio.h>
#include "lotus.h"
#include "lics.h"

int ibm2lics (int bits)
{
int	i;
Lics	*l;

for (i = 0; i < 128; i++) tab[i] = NULL;
for (l = lics; l->sub; l++)
	{
	switch (bits)
	 {
	 case 7:if (l->ct437) tab[l->ct437-128]	= l->sub;
		break;
	 case 8:if (l->ct437) tab[l->ct437-128]	= l->licsc ? &l->licsc : l->sub;
		break;
	 }
	}
}

int lics2ibm (int bits)
{
int	i;
Lics	*l;

for (i = 0; i < 128; i++) tab[i] = NULL;
for (l = lics; l->sub; l++)
	{
	switch (bits)
	 {
	 case 7:if (l->licsc) tab[l->licsc-128]	= l->sub;
		break;
	 case 8:if (l->licsc) tab[l->licsc-128]	= l->ct437 ? &l->ct437 : l->sub;
		break;
	 }
	}
}
