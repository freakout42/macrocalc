/* $Id: mcget.h,v 1.2 1995/02/16 21:57:19 axel Rel $
 */

int getkey (void);
void clearinput (void);
int editstring (char *s, char *legal, int maxlength);
int editstringp (char *s, char *legal, int maxlength, int pos);
