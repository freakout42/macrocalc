/* mcell.h 1.5 2001/07/02 10:23:58 axel */

void freecell (CELLPTR cp);
void inithash (void);
void sheetwalk (CELLPTR (*fcn)(int col, int row));
CELLPTR	cell (int col, int row);
CELLPTR	initcell (	int col, int row,
			unsigned char att, unsigned char form,
			char *s, double val, char *unit);
int deletecell (int col, int row);
int movecell (int tcol, int trow, int scol, int srow);
char *celltext (int col, int row);
int valuecell (int col, int row);
CELLPTR migratcell(CELLPTR ct, CELLPTR st);
