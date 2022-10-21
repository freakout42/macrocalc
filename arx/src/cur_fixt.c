/* $Id: cur_fixt.c,v 1.4 2002/09/07 12:37:10 axel Exp $
 */

#define CURSESBUG
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

#ifdef CURSESBUG
if (keypad_xmit!=NULL) tputs (keypad_xmit, 1, cur_tput);
fflush(stdout);
#endif

#ifdef CURSESBUGX
attrset (A_NORMAL);
move (lines-1, 79);
addch ('.');
refresh();
move (lines-1, 79);
addch (' ');
#endif

#ifndef linux
#ifdef	DEBUG
state.sg_flags |= XTABS;
gtty (1, &state);
fprintf (stderr, "tflags2: %4x\n", state.sg_flags);
#endif
#endif
}
