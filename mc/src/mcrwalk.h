/* $Id: mcrwalk.h,v 1.1 1999/10/24 05:59:08 axel Exp $
 */

void rangewalk (int (*fcn)(int col, int row),
	struct CELLADR cell1, struct CELLADR cell2);
void rangevwalk (int (*fcn)(struct CELLVALUE value),
	struct CELLADR cell1, struct CELLADR cell2);
