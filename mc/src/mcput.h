/* $Id: mcput.h,v 1.1 1995/02/16 21:49:09 axel Rel $
 */

void mcrefresh (void);
void writef (int x, int y, int color, int width, char *format, ...);
void clrtoeob (int x1, int y1, int x2);
