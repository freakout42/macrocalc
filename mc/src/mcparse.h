/* mcparse.h */

extern int origcol, origrow; /* original column */
extern int errpos; /* position of parsing error */
extern CELLPTR pc;
extern char *yybuf;
extern char *yysparse; /* pointer to source formula */
extern char *yybparse; /* pointer to parsed formula */
extern char *yytoparse; /* pointer to left formular */
extern int yyerrorflg; /* error flag for lex-parser */

#ifdef LOTUS
int parse2 (CELLPTR pc, short *formula);
#else
int mcinput (void);
int parse (CELLPTR pc, char *parsed);
#endif
