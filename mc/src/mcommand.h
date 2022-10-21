/* $Id: mcommand.h,v 1.4 1997/05/09 17:35:25 axel Exp $
 */

void newpage (void);
void editcell (CELLPTR ecell);
void setcolwidth (int col);
void gotocell (void);
void formatcells (int and, int or);
void attribcells (int att);
void valuecells (void);
void deletecells (void);
void createrange (void);
void deleterange (void);
void valuerange (void);
void insdelcolrow (int mode);
