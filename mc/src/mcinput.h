/* $Id: mcinput.h,v 1.2 1997/05/09 17:35:25 axel Exp $
 */

int getint (int *number, int low, int high, int def);
int getcell (int *col, int *row);
int getyesno (int *yesno, int prompt);
int generalprompt (void);
