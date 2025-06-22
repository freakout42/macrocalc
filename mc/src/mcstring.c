/* mcstring.c */

#include <string.h>

double length (char *s)
{
return s==NULL ? -1 : (int)strlen(s);
}
