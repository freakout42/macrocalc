/* $Id: cur_exit.c,v 1.4 2024/11/03 10:44:18 axel Exp $ */

#include "stdlib.h"
#include "stdio.h"
#include "arx_def.h"
#include "cur_def.h"
#include <fcntl.h>
#if (COHERENT | linux)
#include <term.h>
#endif

void cur_exit (int code)
{
endwin();
tcsetattr (0, TCSANOW, &otermio);
/*putchar ('\n');*/
if (code!=EOF) exit (code);
}
