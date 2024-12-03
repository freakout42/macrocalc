/* mcfileio.h */

extern int lastcol, lastrow, markcol, markrow, omarkcol, omarkrow;
#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(_STDIO_H_) || defined(WIN32)
int loadfile (FILE *file);
int savefile (FILE *file, int scope);
#endif
