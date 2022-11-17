/* mcell.h 1.5 2001/07/02 10:23:58 axel */

#define	alloctext(col,row,s)	\
	if (initcell (col, row, TEXT, defaultformat, s, .0, NULL) == NULL) \
		errormsg (MSGLOMEM)
#define	allocformula(col,row,s,v) \
	if (initcell (col, row, FORMULA, defaultformat, s, v, NULL) == NULL) \
		errormsg (MSGLOMEM)
#define	allocunit(col,row,u)	\
	 if (initcell (col,row,UNITT,defaultformat|PROTECT,u,.0,NULL) == NULL) \
		errormsg (MSGLOMEM)
#define	alloconstant(col,row,s,v) \
	if (initcell (col, row, CONSTANT, defaultformat, s, v, NULL) == NULL) \
		errormsg (MSGLOMEM)

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
