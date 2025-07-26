/* str_def.h */

#include <wchar.h>

#define str_bpad(s)	{memset (s,' ',sizeof(s)); s[sizeof(s)-1] = ' ';}
#define str_npad(s)	{memset (s,' ',sizeof(s)); s[sizeof(s)-1] = '\0';}

#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(__STDIO_H__) || defined(_STDIO_H_) || defined(_H_STDIO) || defined(WIN32)
char *str_gets (FILE *fp, char *s, size_t n);
char *str_gecs (FILE *fp, char *s, size_t n);
#endif

char *str_trim (char *s);
int str_w2u8(char *tgt, wchar_t *src, int siz);
int utf8_to_ucode(wchar_t *output, char *input, int length);
int str_tlen (char *s);
int str_pos(char *s, int f);
char *str_sub(char *t, char *s, int f, int l, int z);
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
extern int cur_utf8;

#ifdef DUMMY
char *str_repl (char *s, size_t size, const char *old, const char *new);
char *str_del (char *s, unsigned int p, unsigned int a);
int str_abbr (const char *s, const char *a);
char *str_synx (const char *f, const char *s);
double str_stod (const char *s, char **endp);
#endif
