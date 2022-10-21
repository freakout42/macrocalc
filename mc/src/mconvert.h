/* $Id: mconvert.h,v 1.4 2001/11/07 12:23:51 axel Exp $
 */

unsigned char convertformat (unsigned char format);
void convertlabel (Lotrec *r);
void convertcelladr (CELLADR123 *ca123, struct CELLADR *ca);
