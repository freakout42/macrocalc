/* cur_fixt.c */

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

void cur_fixt (void)
{
#ifndef linux
#ifdef	DEBUG
struct sgttyb	state;

gtty (1, &state);
fprintf (stderr, "tflags: %4x\n", state.sg_flags);
#endif
#endif

fixterm();

#ifndef linux
#ifdef	DEBUG
state.sg_flags |= XTABS;
gtty (1, &state);
fprintf (stderr, "tflags2: %4x\n", state.sg_flags);
#endif
#endif
}
