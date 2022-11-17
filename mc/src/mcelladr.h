/* mcelladr.h 1.2 1997/04/04 06:29:40 */

CELLPTR	newcell (void);
int coladr (char *s, int *col);
int rowadr (char *s, int *row);
int cellaadr (char *s, int *col, int *row);
int celladr (char *s, int *col, int *row);
int rangeadr (char *s, struct Range *range);
int colstring (int col, char *colstr);
char *celladrstring (int col, int row, char *s);
