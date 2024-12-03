/* str_repl.c,v 1.3 2015/02/05 09:23:57 axel
 * Replace chars like tr
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <str_def.h>

char *str_tr (char *v, char s, char t) {
char *i;

while((i = strchr(v, s))) {
  *i = t;
}
return(v);
}

char *str_repl (char *v, char *s, char *t
#ifdef FIND_THEM_ALL
, char *i
#endif
)
{
register char *p, *q;
char *v1;

v1 = NULL;
for (p = v; *p; p++)
  if ((q = strchr(s, *p))) {
#ifdef FIND_THEM_ALL
    fprintf(stderr, "%3d %c\n", (unsigned char)(*p), i[q - s]);
#endif
    *p = t[q - s];
    v1 = v;
  }
return (v1);
}

#ifdef TEST
main()
{
	char buf[80];

	while (NULL != gets(buf))
		printf("'%s'\n", str_repl(buf, "hello", "gmoin"
#ifdef FIND_THEM_ALL
, "malsn"
#endif
));
}
#endif
