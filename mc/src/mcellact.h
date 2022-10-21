/* $Id: mcellact.h,v 1.6 1999/10/24 05:58:08 axel Exp $
 */

int inrange (int col, int row, int scol, int srow, int ecol, int erow);
int copycell (int tcol, int trow, int scol, int srow);
void moverange (int tcol, int trow, int scol, int srow, int ecol, int erow);
void copyrange (int tcol, int trow, int scol, int srow, int ecol, int erow);
double cellvalue (int col, int row);
void deletecol (int col);
void insertcol (int col);
void deleterow (int row);
void insertrow (int row);
