/* mcmenu.c */

#include <stdio.h>
#include <stddef.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcgetmnu.h"
#include "mcellact.h"
#include "mcwindow.h"
#include "mcommand.h"
#include "mcmessag.h"
#include "mcolor.h"
#include "mcfile.h"
#include "mcdisply.h"	/* not nice! */
#include "mchelp.h"
#include "mcact.h"
#include "mcput.h"
#include "mcfileio.h"
#include "mcmenu.h"

char *mnu[MNUEND] = {NULL};

static void wimenu (int mode)
/* Executes the commands in the insert/delete menu */
{
switch (getmenu(WIMENU))
 {
 case 0:
	if (curcol>lastcol) break;
	if (mode==INSERT)	insdelcolrow(INSCOL);
	else			insdelcolrow(DELCOL);
	break;
 case 1:
	if (currow>lastrow) break;
	if (mode==INSERT)	insdelcolrow(INSROW);
	else			insdelcolrow(DELROW);
	break;
 }
} /* wimenu */

static void wtmenu (void)
/* Executes the commands in the titels menu */
{
switch (getmenu(WTMENU))
 {
 case 0:
	horizwindow();
	break;
 case 1:
	clearwindow();
	break;
 }
} /* wtmenu */

static void wmenu (void)
/* Executes the commands in the worksheet menu */
{
switch (getmenu(WMENU))
 {
 case 0:
	wimenu(INSERT);
	break;
 case 1:
	wimenu(DELETI);
	break;
 case 2:
	setcolwidth(curcol);
	break;
 case 3:
	changeautocalc(!autocalc);
	break;
 case 4:
	changeformdisplay(!formdisplay);
	break;
 case 5:
	checkforsave();
	clearsheet();
	displayscreen(UPDATE);
	break;
 case 6:
	wtmenu();
	break;
 case 7:
	newpage();
	break;
 }
} /* wmenu */

static void rfmenu (void)
/* Executes the commands in the format menu */
{
switch (getmenu(RFMENU))
 {
 case 0:
	formatcells(PROTECT, FIXED);
	break;
 case 1:
	formatcells(PROTECT, SCIENTIFIC);
	break;
 case 2:
	formatcells(PROTECT, CURRENCY|2);
	break;
 case 3:
	formatcells(PROTECT, COMMA);
	break;
 case 4:
	formatcells(PROTECT, PERCENT);
	break;
 case 5:
	formatcells(PROTECT, SPECIAL|DATE);
	break;
 case 6:
	formatcells(PROTECT, SPECIAL|EDATE);
	break;
 case 7:
	formatcells(PROTECT, SPECIAL|RDBDATE);
	break;
 case 8:
	formatcells(PROTECT, SPECIAL|TIME);
	break;
 case 9:
	formatcells(PROTECT, SPECIAL|HIDDEN);
	break;
 case 10:
	formatcells(PROTECT, SPECIAL|DEFAULT);
	break;
 }
} /* rfmenu */

static void ramenu (void)
/* Executes the commands in the attribute menu */
{
switch (getmenu(RAMENU))
 {
 case 0:
	attribcells(BOLD);
	break;
 case 1:
	attribcells(ITALIC);
	break;
 case 2:
	attribcells(0);
	break;
 }
} /* ramenu */

static void comenu (void)
/* Executes the commands in the attribute menu */
{
switch (getmenu(COMENU))
 {
 case 0:
  attribcells(COL_BLACK);
  break;
 case 1:
  attribcells(COL_RED);
  break;
 case 2:
  attribcells(COL_GREEN);
  break;
 case 3:
  attribcells(COL_YELLOW);
  break;
 case 4:
  attribcells(COL_BLUE);
  break;
 case 5:
  attribcells(COL_MAGENTA);
  break;
 case 6:
  attribcells(COL_CYAN);
  break;
 case 7:
  attribcells(COL_WHITE);
  break;
 case 8:
	attribcells(0);
	break;
 }
} /* comenu */

static void rnmenu (void)
/* Executes the commands in the name menu */
{
switch (getmenu(RNMENU))
 {
 case 0:
	createrange();
	break;
 case 1:
	deleterange();
	break;
 }
} /* rnmenu */

static void rmenu (void)
/* Executes the commands in the range menu */
{
switch (getmenu(RMENU))
 {
 case 0:
	rfmenu();
	break;
 case 1:
	ramenu();
	break;
 case 2:
	comenu();
	break;
 case 3:
	deletecells ();
	break;
 case 4:
	rnmenu();
	break;
 case 5:
	formatcells(FORMATM|PLACES, PROTECT);
	break;
 case 6:
	formatcells(FORMATM|PLACES, 0);
	break;
 case 7:
	valuecells();
	break;
 }
} /* rmenu */

extern int cur_utf8;
static void fmenu (void)
/* Executes the commands in the file menu */
{
#ifndef NOUTF8
char version[132];
#endif
switch (getmenu (FMENU))
 {
 case 0:
	savesheet(TRUE);
	break;
 case 1:
	saverange();
	break;
 case 2:
	checkforsave();
	clearsheet();
	/*FALLTHRU*/
 case 3:
	loadsheet(PROMPT);
	break;
 case 4:
	savesheet(2);
	break;
 case 5:
	checkforsave();
	clearsheet();
	loadsheet(2);
	break;
 case 6:
#ifndef NOUTF8
  snprintf(version, 130, "%s w=%c", rcsinfo, cur_utf8 ? '1' : '0');
	errorstr(version);
#else
	errorstr(rcsinfo);
#endif
	break;
 }
} /* fmenu */

void mainmenu (void)
/* Executes the commands in the main menu */
{
switch (getmenu(MENU))
 {
 case 0:
	wmenu();
	break;
 case 1:
	rmenu();
	break;
 case 2:
	copyrange(curcol, currow, omarkcol, omarkrow, markcol, markrow);
	displayscreen(NOUPDATE);
	break;
 case 3:
	moverange(curcol, currow, omarkcol, omarkrow, markcol, markrow);
	displayscreen(NOUPDATE);
	break;
 case 4:
	fmenu();
	break;
 case 5:
	printsheet(PROMPT);
	break;
 case 6:
	mcrefresh();
	break;
 case 7:
#ifdef WIN32
	mcsystem("CMD", FALSE);
#else
	mcsystem("${SHELL-/bin/sh}", FALSE);
#endif
	break;
 case 8:
	if (!checkforsave()) stop = TRUE;
	break;
 }
} /* mainmenu */
