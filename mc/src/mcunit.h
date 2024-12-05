/* mcunit.h */

int unitinit (void);
int unitclose (void);
double unitconv (double s, char *su, char *tu);
char *unitnorm (char *n, char *u);
char *unitinv (char *n, char *u);
char *unitmult (char *n, char *u1, char *u2);
char *unitdiv (char *n, char *u1, char *u2);
int binary();
void mcunitconvert(char *inbuf, char *su, char *tu);
