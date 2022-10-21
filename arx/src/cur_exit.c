/* $Id: cur_exit.c,v 1.3 1995/02/22 18:01:13 axel Rel $
 */

#include "stdlib.h"
#include "stdio.h"
#include "arx_def.h"
#include "cur_def.h"

void cur_exit (int code)
{
endwin();
putchar ('\n');
if (code!=EOF) exit (code);
}
