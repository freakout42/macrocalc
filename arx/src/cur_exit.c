/* cur_exit.c */

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
/*tcsetattr (0, TCSANOW, &otermio); handled by endwin() because settings after initscr() */
/*putchar ('\n');*/
if (code!=EOF) exit (code);
}
