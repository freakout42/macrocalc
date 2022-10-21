/* $Id: mcfileio.h,v 1.3 2004/05/16 08:24:44 axel Exp $ */

extern int lastcol, lastrow, markcol, markrow, omarkcol, omarkrow;
#if defined(_STDIO_H) || defined(_STDIO_INCLUDED)
int loadfile (FILE *file);
int savefile (FILE *file, int scope);
#endif
