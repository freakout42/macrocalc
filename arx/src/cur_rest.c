/* cur_rest.c */

#include <stdio.h>
#include <errno.h>
#include <curses.h>
#include "arx_def.h"
#include "cur_def.h"
#include <term.h>
#ifndef linux
#ifdef DEBUG
#include <sgtty.h>
#endif
#endif

void cur_rest (void)
{
#ifndef linux
#ifdef	DEBUG
struct sgttyb	state;

gtty (1, &state);
fprintf (stderr, "tflags: %4x\n", state.sg_flags);
#endif
#endif

saveterm();
resetterm();

cur_goll();
cur_tput('\n');
}
