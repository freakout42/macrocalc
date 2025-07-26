/* mcdisply.h */

void printnull (void);
void printcol (void);
void clearlastcol (void);
void printrow (void);
void displaycell (int col, int row, int highlighting, int updating);
void displaycol (int col, int updating);
void displayrow (int row, int updating);
void displaycur (int highlighting);
void displayscreen (int updating);
void showcelltype (void);
void redrawscreen (void);
void displaymark (void);
void changeautocalc (int newmode);
void changeformdisplay (int newmode);
void mcsystem (char *command, int prompt);
