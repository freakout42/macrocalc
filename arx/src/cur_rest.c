/* $Id: cur_rest.c,v 1.6 2012/02/20 20:00:35 axel Exp $
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

#ifdef CURSESBUG
if (exit_attribute_mode!=NULL) tputs (exit_attribute_mode, 1, cur_tput);
#if NCURSES_VERSION_MAJOR < 5
curscr->_attrs = A_NORMAL;
#endif
#endif

cur_goll();
cur_tput('\n');
}
