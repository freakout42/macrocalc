/* $Id: mcrecalc.c,v 1.5 1997/05/21 15:26:58 axel Exp $
 */

#include <string.h>
#include "mc.h"
#include "mccalc.h"
#include "mcdisply.h"
#include "mcgetmsg.h"
#include "mcmessag.h"
#include "mcrecalc.h"

void recalc (void)
/* Recalculates all of the numbers in the speadsheet */
{
static char	*msgrecalc	= NULL;
char		m[MAXMSGWIDTH+1];

if (!changed) return;
if (msgrecalc == NULL)
	{
	getmessage (m, MSGRECALC);
	msgrecalc	= strdup (m);
	}
if (menudisp) dmessage (msgrecalc);
recalcworksheet();
displayscreen (UPDATE);
} /* recalc */

void autorecalc (void)
{
if (autocalc) recalc();
}
