/* $Id: mcparse.h,v 1.4 1999/10/18 09:55:32 axel Exp $
 */

extern int origcol, origrow; /* original column */
extern int errpos; /* position of parsing error */
extern double yyvalue;
extern int yytype;
extern char *yyunit;
extern char *yybuf;
extern char *yysparse; /* pointer to source formula */
extern char *yybparse; /* pointer to parsed formula */
extern char *yytoparse; /* pointer to left formular */
extern int yyerrorflg; /* error flag for lex-parser */

#ifdef LOTUS
int parse2 (char *s, short *formula);
#else
int mcinput (void);
double parse (char *s, int *t, char *unit, char *parsed);
#endif
