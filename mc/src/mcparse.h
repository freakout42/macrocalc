/* $Id: mcparse.h,v 1.4 1999/10/18 09:55:32 axel Exp $
 */

extern int origcol, origrow;
#ifdef LOTUS
int parse2 (char *s, short *formula);
#else
int mcinput (void);
double parse (char *s, int *t, char *unit, char *parsed);
#endif
