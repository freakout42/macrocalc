/* mcrun.c */

#include <stdio.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcell.h"
#include "mcinput.h"
#include "mchelp.h"
#include "mcommand.h"
#include "mcrmacro.h"
#include "mcrecalc.h"
#include "mcmenu.h"
#include "mcact.h"
#include "mcdisply.h"
#include "mcmove.h"
#include "mcfile.h"
#include "mcfileio.h"
#include "mcrun.h"

void run (void)
/* The main program loop */
{
int input;
int lmarkcol= 0;
int lmarkrow= 0;

do {
  curcell = cell (curcol, currow);
  input = generalprompt();
  if (input < 0) input *= -1;
  switch (input)
   {
   case F1 :
   case '?' :
    mchelp();
    break;
   case F2 :
   case '!' :
    changed |= act('\0');
    break;
   case F3 :
   case '.' :
    omarkcol	= min (lmarkcol, curcol);
    omarkrow	= min (lmarkrow, currow);
    markcol	= max (lmarkcol, curcol);
    markrow	= max (lmarkrow, currow);
    lmarkcol	= curcol;
    lmarkrow	= currow;
    if (highlight) displayscreen (NOUPDATE);
    break;
   case F4 :
   case '%' :
    omarkcol	= 0;
    omarkrow	= 0;
    markcol	= lastcol;
    markrow	= lastrow;
    if (highlight) displayscreen (NOUPDATE);
    break;
   case F5 :
   case '>' :
    gotocell();
    break;
   case F6 :
   case '$' :
    runmacro(PROMPT);
    break;
   case F8 :
   case '#' :
   case SUSPEND :
    savesheet(FALSE);
    if (changed) *filename = '\0'; else stop = TRUE;
    break;
   case CANCEL :
    if (!checkforsave()) stop = TRUE;
    break;
   case F9 :
   case '=' :
    recalc();
    break;
   case F10 :
   case '/' :
   case '<' :
    mainmenu();
    break;
   case DELKEY :
    deletecell (curcol, currow);
    changed = TRUE;
    autorecalc();
    break;
   case PGUPKEY :
    movepageup();
    break;
   case PGDNKEY :
    movepagedown();
    break;
   case CTRLLEFTKEY :
    movepageleft();
    break;
   case CTRLRIGHTKEY :
    movepageright();
    break;
   case HOMEKEY :
    movehome();
    break;
   case ENDKEY :
    moveend();
    break;
   case UPKEY :
    moverowup();
    break;
   case DOWNKEY :
    moverowdown();
    break;
   case LEFTKEY :
    movecolleft();
    break;
   case RIGHTKEY :
    movecolright();
    break;
   default :
    if ((input >= ' ') && (input <= '~'))
      changed |= act(input);
    break;
   } /* switch */
  } while (!stop);
} /* run */
