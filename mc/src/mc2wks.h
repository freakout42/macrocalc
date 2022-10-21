/* $Id: mc2wks.h,v 1.4 1995/03/09 20:09:42 axel Rel $
 */

void fatal (int x, char *s);
int fromwks (FILE *file);
int towks (FILE *file);
int ibm2lics (int bits);
int lics2ibm (int bits);
int pol2tex (char *tex, unsigned char *code);
