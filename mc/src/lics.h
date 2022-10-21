/* $Id: lics.h,v 1.1 1995/03/08 19:34:51 axel Rel $
 * Lotus LICS-codes table
 */

typedef struct lics	/* lics table structure			*/
	{
	char	comb[2];
	char	uxseq[2];
	char	*sub;
	unsigned char	licsc;
	unsigned char	ct437;
	} Lics;

extern Lics lics[];

extern unsigned char	*tab[128];
