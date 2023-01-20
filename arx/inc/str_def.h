/* str_def.h,v 1.9 2015/02/05 09:23:57 axel */

#define str_bpad(s)	{memset (s,' ',sizeof(s)); s[sizeof(s)-1] = ' ';}
#define str_npad(s)	{memset (s,' ',sizeof(s)); s[sizeof(s)-1] = '\0';}

#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(__STDIO_H__) || defined(_STDIO_H_) || defined(_H_STDIO)
char *str_gets (FILE *fp, char *s, size_t n);
char *str_gecs (FILE *fp, char *s, size_t n);
#endif

char *str_trim (char *s);
int str_tlen (char *s);
char *str_upr (char *s);
char *str_low (char *s);
char *str_tr (char *v, char s, char t);
#ifdef FIND_THEM_ALL
char *str_repl (char *v, char *s, char *t, char *i);
#else
char *str_repl (char *v, char *s, char *t);
#endif
double str_chkd (const char *s);
char *str_inc (char *s, const char ic);
char *str_ins (char *s, char *i);

#ifdef DUMMY
char *str_repl (char *s, size_t size, const char *old, const char *new);
char *str_del (char *s, unsigned int p, unsigned int a);
int str_abbr (const char *s, const char *a);
char *str_synx (const char *f, const char *s);
double str_stod (const char *s, char **endp);
#endif
