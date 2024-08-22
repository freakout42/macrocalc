/* main.c
 * Original code written by Dave G. Conroy,
 * substantially modified by Moshe Braner, July-December 1986.
 * Further substantial modifications by MB: January and April, 1988.
 *
 * Parts of the program Copyright (c) 1986, 1988 by Moshe Braner.
 * Program name changed from microEMACS to MEX.
 *
 * Ported to Coherent 3.1 and UN*X System V
 * Copyright (c) 1991 by Udo Munk
 *
 * Permission is granted to copy and use this program,
 * provided that this copyright notice and the source
 * code for the program are included in each copy,
 * and that it is not sold for profit.
 */

/*
 * This file contains the
 * main driving routine, and some
 * keyboard processing code, for the
 * MicroEMACS screen editor.
 */
#include	<stdio.h>
#include	<string.h>
#include	"ed.h"
void edmore(char fname[]);

#if CURSES
#include	"curkeys.h"
#endif

#if ST_DA
#include	 <obdefs.h>
#include	 <gemdefs.h>
#endif

#if (MSDOS | W32)
#include	<fcntl.h>
#include	<conio.h>
#endif

#if	VMS
#include	<ssdef.h>
#define GOOD	(SS$_NORMAL)
#endif

#ifndef GOOD
#define GOOD	0
#endif

#define BASE	0		/* mb: states of the machine */
#define ESC	1
#define ARG	2
#define EXEC	3
#define DASTART	990		/* starting the DA	*/
#define DACLOSE	991		/* closing the DA	*/

char	*rcsid = "$Id: main.c,v 1.41 2024/05/22 17:56:04 axel Exp $";
int	logit = LOGIT;			/* mb: log keystrokes		*/
int	playback = FALSE;		/* mb: playback from log file	*/
#if ST_DA
char	*clfn[NFILES];
#else
char	**clfn;				/* mb: filenames storage	*/
#endif
int	nfiles;				/* mb: number of file args	*/
int	maxnfiles;			/* mb: max # of fn defaults	*/
int	fileindex;			/* mb: index into filename list	*/
int	currow;				/* Working cursor row		*/
int	curcol;				/* Working cursor column	*/
int	fillcol = 0;			/* Current fill column		*/
int	lmargin = 0;			/* mb: Current left margin	*/
int	tabsize = 2;			/* mb: added			*/
int	hdev;				/* mb: hard copy device	ID	*/
int	thisflag;			/* Flags, this command		*/
int	lastflag;			/* Flags, last command		*/
int	curgoal;			/* Goal column			*/
int	ovrstrk = FALSE;		/* mb: insert/overstrike flag	*/
int	deldir  = FALSE;		/* mb: <Delete> direction flag	*/
int	casesens = FALSE;		/* mb: search case sensitive	*/
BUFFER  *curbp;				/* Current buffer		*/
BUFFER  *oldbp = NULL;			/* mb: previous buffer		*/
WINDOW  *curwp;				/* Current window		*/
BUFFER  *bheadp = NULL;			/* BUFFER listhead		*/
WINDOW  *wheadp;			/* WINDOW listhead		*/
BUFFER  *blistp;			/* Buffer list BUFFER		*/
BUFFER  *bhelpp;			/* Help screens BUFFER		*/
int	kbdm[NKBDM];			/* Macro			*/
int	*kbdmip;			/* Input  for above		*/
int	*kbdmop;			/* Output for above		*/
char	pat[NPAT] = {'\0'};		/* Pattern			*/

#if AtST
int	vidrev	= FALSE;		/* may be changed by cmd line	*/
#endif
#if (MSDOS)
int	origvidmode;			/* save on entry		*/
int	nrmlvidattr;			/* normal in our vidmode	*/
int	curvidattr;			/* update all along		*/
int	vidrev	  = FALSE;		/* may be changed by cmd line	*/
int	vidmode   = LASTMODE;		/* may be changed by cmd line	*/
#endif

#if ST_DA
extern int	gl_apid;
int	menu_id, event, ret;
int	xdesk, ydesk, hdesk, wdesk;
int	msgbuff[8];
char	menusave[1520];
#endif

typedef struct  {
	int	k_code;			/* Key code			*/
	int	(*k_fp)();		/* Routine to handle it		*/
}	KEYTAB;

extern  int	mlrelpy();		/* get a string from the user	*/
extern  int	quit();			/* Quit				*/
extern  int	ctlxlp();		/* Begin macro			*/
extern  int	ctlxrp();		/* End macro			*/
extern  int	ctlxe();		/* Execute macro		*/
extern  int	fileread();		/* Get a file, read only	*/
extern  int	filevisit();		/* Get a file, read write	*/
extern  int	filewrite();		/* Write a file			*/
extern  int	filesave();		/* Save current file		*/
extern  int	filename();		/* Adjust file name		*/
extern  int	getccol();		/* Get current column		*/
extern  int	gotobol();		/* Move to start of line	*/
extern  int	forwchar();		/* Move forward by characters	*/
extern  int	gotoeol();		/* Move to end of line		*/
extern  int	backchar();		/* Move backward by characters  */
extern  int	forwline();		/* Move forward by lines	*/
extern  int	backline();		/* Move backward by lines	*/
extern  int	forwpage();		/* Move forward by pages	*/
extern  int	backpage();		/* Move backward by pages	*/
extern  int	gotobob();		/* Move to start of buffer	*/
extern  int	gotoeob();		/* Move to end of buffer	*/
extern  int	setfillcol();		/* Set fill column.		*/
extern  int	setlmargin();		/* mb: added			*/
extern  int	reformat();		/* mb: added			*/
extern  int	setmark();		/* Set mark			*/
extern  int	swapmark();		/* Swap "." and mark		*/
extern  int	forwsearch();		/* Search forward		*/
extern  int	backsearch();		/* Search backwards		*/
extern  int	showcpos();		/* Show the cursor position	*/
extern  int	nextwind();		/* Move to the next window	*/
extern  int	prevwind();		/* Move to the previous window  */
extern  int	onlywind();		/* Make current window only one */
extern  int	splitwind();		/* Split current window		*/
extern  int	mvdnwind();		/* Move window down		*/
extern  int	mvupwind();		/* Move window up		*/
extern  int	enlargewind();		/* Enlarge display window.	*/
extern  int	listbuffers();		/* Display list of buffers	*/
extern  int	usebuffer();		/* Switch a window to a buffer  */
extern  int	killbuffer();		/* Make a buffer go away.	*/
extern  int	reposition();		/* Reposition window		*/
extern  int	negrepos();		/* Reposition window other way	*/
extern  int	refresh();		/* Refresh the screen		*/
extern  int	twiddle();		/* Twiddle characters		*/
extern  int	ltwiddle();		/* Twiddle lines - mb: added	*/
extern  int	tab();			/* Insert tab			*/
extern  int	tnewline();		/* Insert CR-LF			*/
extern  int	indent();		/* Insert CR-LF, then indent	*/
extern  int	openline();		/* Open up a blank line		*/
extern  int	deblank();		/* Delete blank lines		*/
extern  int	backword();		/* Backup by words		*/
extern  int	forwword();		/* Advance by words		*/
extern  int	forwdel();		/* Forward delete		*/
extern  int	backdel();		/* Backward delete		*/
extern  int     killtxt();              /* Kill forward                 */
extern  int	yank();			/* Yank back from killbuffer.	*/
extern  int	unyank();		/* mb: added			*/
extern  int	upperword();		/* Upper case word		*/
extern  int	lowerword();		/* Lower case word		*/
extern  int	upperregion();		/* Upper case region		*/
extern  int	lowerregion();		/* Lower case region		*/
extern  int	capword();		/* Initial capitalize word	*/
extern  int	delfword();		/* Delete forward word		*/
extern  int	delbword();		/* Delete backward word		*/
extern  int	killregion();		/* Kill region			*/
extern  int	copyregion();		/* Copy region to kill buffer	*/
extern  int	spawncli();		/* Run CLI in a subjob		*/
extern  int	spawn();		/* Run a command in a subjob	*/
extern  int	quickexit();		/* low keystroke style exit	*/
extern	int	hardcopy();		/* mb: added. Output to printer	*/
extern  int	wrapword();		/* mb: added the rest, 		*/
extern  int	help();			/*   just in case      		*/
extern  int	forw_brace();
extern  int	back_brace();
extern  int	instog();
extern  int	casestog();
extern  int	fortog();
extern  int	editog();
extern  int	visitog();
extern  int	gotolinum();
extern  int	forwparag();
extern  int	backparag();
extern  int	renambuf();
extern  int	page_nextw();
extern  int	back_nextw();
extern  int	flush_kbuf();
extern  int	doplay();		/* fwd ref */
extern  int	emacs_quit();		/* fwd ref */
extern  int	ctrlg();		/* fwd ref */
extern  int	undo();			/* fwd ref */
extern  int	bkill();		/* fwd ref */
extern  int	fbdel();		/* fwd ref */
extern  int	togdeldir();		/* fwd ref */
extern  int	defmacro();		/* fwd ref */

/*
 * Command table.
 * This table  is *roughly* in ASCII
 * order, left to right across the characters
 * of the command. This expains the funny
 * location of the control-X commands.
 */
KEYTAB  keytab[] = {
	CNTL|'@',		setmark,
	CNTL|'A',		gotobol,
	CNTL|'B',		backchar,
	CNTL|'C',		quit,		/* mb: was C-X C-C */
	0x163,			quit,		/* CentOS5.2 */
     ED|CNTL|'D',		forwdel,
	CNTL|'E',		gotoeol,
	CNTL|'F',		forwchar,
	CNTL|'G',		undo,
     ED|CNTL|'H',		backdel,
#if (MSDOS | W32)
     ED|CNTL|0x3F,		bkill,		/* mb: ^BS */
#else
     ED|CNTL|0x3F,		fbdel,		/* mb: DELETE key */
#endif
     ED|CNTL|'I',		tab,
#if defined(VT100) || defined(HP700)
     ED|CNTL|'J',		tnewline,
#else
     ED|CNTL|'J',		indent,
#endif
     ED|CNTL|'K',               killtxt,
	CNTL|'L',		refresh,
     ED|CNTL|'M',		tnewline,
	CNTL|'N',		forwline,
     ED|CNTL|'O',		instog,		/* openline, */
	CNTL|'P',		backline,
	CNTL|'S',		forwsearch,
	CNTL|'R',		backpage,
     ED|CNTL|'T',		twiddle,
	CNTL|'V',		forwpage,
     ED|CNTL|'W',		killregion,
     ED|CNTL|'Y',		yank,
	CNTL|'Z',		quickexit,	/* quick save and exit  */
	0x197,			quickexit,	/* CentOS5.2 */
	CTLX|CNTL|'B',		renambuf,	/* mb: added */
	CTLX|CNTL|'C',		spawncli,	/* Run CLI in subjob.	*/
	CTLX|CNTL|'D',		fortog,		/* ar: added */
	CTLX|CNTL|'E',		editog,		/* mb: added */
	CTLX|CNTL|'F',		filename,
	CTLX|CNTL|'R',		fileread,
#if V7
	/* skip */
#else
     ED|CTLX|CNTL|'S',		filesave,
#endif
	CTLX|CNTL|'V',		filevisit,
     ED|CTLX|CNTL|'W',		filewrite,
	CTLX|CNTL|'X',		swapmark,
     ED|CTLX|CNTL|'Y',		unyank,		/* mb: added */
	CTLX|CNTL|'Z',		emacs_quit,
	CTLX|CNTL|0x3F,		togdeldir,	/* DELETE key dir */
	CTLX|'?',		listbuffers,
	CTLX|'!',		spawn,
#if CANLOG
	CTLX|'&',		doplay,
#endif
	CTLX|'=',		showcpos,
	CTLX|'(',		ctlxlp,
	CTLX|')',		ctlxrp,
	CTLX|'1',		onlywind,
	CTLX|'2',		splitwind,
	CTLX|'B',		usebuffer,
	CTLX|'C',		casestog,
	CTLX|'E',		ctlxe,
     ED|CTLX|'F',		reformat,	/* mb: added */
	CTLX|'K',		killbuffer,
     ED|CTLX|'L',		setlmargin,	/* mb: added */
	CTLX|'M',		defmacro,	/* mb: added */
	CTLX|'N',		nextwind,
	CTLX|'O',		nextwind,	/* mb: EMACS-like */
	CTLX|'P',		prevwind,
	CTLX|'Q',		visitog,	/* mb: added */
     ED|CTLX|'R',		setfillcol,
     ED|CTLX|'S',		filesave,	/* mb: instead of ^X^S  */
     ED|CTLX|'T',		ltwiddle,	/* mb: added */
	CTLX|'V',		page_nextw,	/* mb: added */
	CTLX|'Z',		back_nextw,	/* mb: added */
	META|'!',		reposition,
#if	VT100
	/* skip this - it clashes with function key codes */
#else
	META|']',		forw_brace,	/* mb: added */
	META|'[',		back_brace,	/* mb: added */
#endif
	META|')',		forw_brace,	/* mb: added */
	META|'}',		forw_brace,	/* mb: added */
	META|'{',		back_brace,	/* mb: added */
	META|'(',		back_brace,	/* mb: added */
	META|'.',		setmark,
	META|' ',		setmark,
	META|'>',		gotoeob,
	META|'<',		gotobob,
	META|'B',		backword,
     ED|META|'C',		capword,
     ED|META|'D',		delfword,
	META|'F',		forwword,
	META|'G',		gotolinum,	/* mb: added */
     ED|META|'I',		instog,		/* mb: added */
     ED|META|'L',		lowerword,
	META|'R',		backsearch,
	META|'S',		forwsearch,
     ED|META|'U',		upperword,
	META|'V',		backpage,
	META|'W',		copyregion,
	META|'Y',		flush_kbuf,
     ED|META|CNTL|'H',		delbword,
     ED|META|CNTL|0x3F,		fbwdel,

#if	EXTRA
     ED|CTLX|CNTL|'L',		lowerregion,
	CTLX|CNTL|'N',		mvdnwind,
     ED|CTLX|CNTL|'O',		deblank,
	CTLX|CNTL|'P',		mvupwind,
     ED|CTLX|CNTL|'U',		upperregion,
	CTLX|'^',		enlargewind,
	META|'N',		forwparag,	/* mb: added */
	META|'P',		backparag,	/* mb: added */
#endif

#if	(AtST | MSDOS | W32)
	CTLX|CNTL|'H',		hardcopy,	/* mb: added */
#endif

#if	AtST					/* mb: added */

	FUNC|0x4B,		backchar,	/* <-- */
	FUNC|SHFT|0x4B,		backword,
	FUNC|SHFT|0x73,		backword,
	FUNC|META|0x4B,		gotobol,
	FUNC|0x4D,		forwchar,	/* --> */
	FUNC|SHFT|0x4D,		forwword,
	FUNC|SHFT|0x74,		forwword,
	FUNC|META|0x4D,		gotoeol,
	FUNC|0x50,		forwline,
	FUNC|0x48,		backline,
#if	EXTRA
	FUNC|SHFT|0x50,		mvdnwind,
	FUNC|SHFT|0x48,		mvupwind,
	FUNC|META|0x50,		forwparag,
	FUNC|META|0x48,		backparag,
#endif
	FUNC|0x61,		undo,   	/* Undo */
	FUNC|SHFT|0x61,		flush_kbuf,
     ED|FUNC|0x53,		fbdel,		/* Del */
     ED|FUNC|SHFT|0x53,         killtxt,
	CTLX|FUNC|0x53,		togdeldir,
     ED|FUNC|0x0E,		backdel,	/* Backspace */
     ED|FUNC|SHFT|0x0E,		bkill,
     ED|FUNC|META|0x53,		fbwdel,		/* Delete */
     ED|FUNC|META|0x0E,		delbword,	/* Backspace */
	FUNC|0x47,		reposition,	/* Clr/Home  */
	FUNC|SHFT|0x47,		negrepos,	/* w/shift   */
	FUNC|META|0x47,		gotobob,	/* Esc-Home  */
	FUNC|SHFT|0x77,		gotoeob,	/* Ctrl-Home */
     ED|FUNC|0x52,		openline,	/* Insert */
#if EXTRA
     ED|FUNC|SHFT|0x52,		deblank,
#endif
	FUNC|0x3B,		ctlxe,		/* F1  */
	FUNC|SHFT|0x3B,		defmacro,
	FUNC|0x3C,		usebuffer,	/* F2  */
	FUNC|SHFT|0x3C,		filevisit,
	FUNC|0x3D,		fileread,	/* F3  */
	FUNC|SHFT|0x3D,		filewrite,
     ED|FUNC|0x3E,              killtxt,        /* F4  */
     ED|FUNC|SHFT|0x3E,		killregion,
     ED|FUNC|0x3F,		yank,		/* F5  */
	FUNC|SHFT|0x3F,		copyregion,
	FUNC|0x40,		backsearch,	/* F6  */
	FUNC|SHFT|0x40,		back_brace,
	FUNC|0x41,		forwsearch,	/* F7  */
	FUNC|SHFT|0x41,		forw_brace,
	FUNC|0x42,		backpage,	/* F8  */
	FUNC|SHFT|0x42,		back_nextw,
	FUNC|0x43,		forwpage,	/* F9  */
	FUNC|SHFT|0x43,		page_nextw,
	FUNC|0x44,		setmark,	/* F10 */
	FUNC|SHFT|0x44,		swapmark,
#if	HELP
	FUNC|0x62,		help,		/* Help */
#else
	FUNC|0x62,		listbuffers,
#endif
	FUNC|SHFT|0x62,		listbuffers,
    /* Alt to act as Ctrl-X-Ctrl: */
	FUNC|SHFT|0x12,		editog,		/* Alt-E */
     ED|FUNC|SHFT|0x14,		ltwiddle,	/* Alt-T */
	FUNC|SHFT|0x2D,		swapmark,	/* Alt-X */
    /* Alt to act as Ctrl-X: */
	FUNC|SHFT|0x83,		showcpos,
	FUNC|SHFT|0x78,		onlywind,
	FUNC|SHFT|0x79,		splitwind,
	FUNC|SHFT|0x30,		usebuffer,
     ED|FUNC|SHFT|0x21,		reformat,
	FUNC|SHFT|0x25,		killbuffer,
     ED|FUNC|SHFT|0x26,		setlmargin,
	FUNC|SHFT|0x32,		defmacro,
	FUNC|SHFT|0x31,		nextwind,
	FUNC|SHFT|0x19,		prevwind,
     ED|FUNC|SHFT|0x13,		setfillcol,
	FUNC|SHFT|0x10,		visitog,
     ED|FUNC|SHFT|0x1F,		filesave,
	FUNC|SHFT|0x2F,		filevisit,
    /* Other Alt- functions: */
	FUNC|SHFT|0x2E,		casestog,	/* Alt-C */
	FUNC|SHFT|0x20,		spawncli,	/* Alt-D */
	FUNC|SHFT|0x22,		gotolinum,	/* Alt-G */
     ED|FUNC|SHFT|0x17,		instog,		/* Alt-I */
     ED|FUNC|SHFT|0x18,		openline,	/* Alt-O */
     ED|FUNC|SHFT|0x11,		copyregion,	/* Alt-W */
     ED|FUNC|SHFT|0x15,		yank,		/* Alt-Y */
	FUNC|SHFT|0x80,		back_brace,	/* Alt-( */
	FUNC|SHFT|0x81,		forw_brace,	/* Alt-) */
#if CANLOG
	FUNC|SHFT|0x7E,		doplay,		/* Alt-7 */
#endif
#if	EXTRA
	FUNC|SHFT|0x7D,		enlargewind,	/* Alt-^ */
#endif
    /* Still available:
	FUNC|SHFT|0x23,		Alt-H
	FUNC|SHFT|0x24,		Alt-J
	FUNC|SHFT|0x16,		Alt-U
	FUNC|SHFT|0x2C,		Alt-Z
	FUNC|SHFT|0x7A,		Alt-3
	FUNC|SHFT|0x7B,		Alt-4
	FUNC|SHFT|0x7C,		Alt-5
	FUNC|SHFT|0x7F,		Alt-8
	FUNC|SHFT|0x82,		Alt '-'

	FUNC|SHFT|0x??,		spawn,		Alt-?
     */
#endif

#if	(MSDOS | W32)
	FUNC|0x4B,		backchar,	/* <-- */
	FUNC|0x73,		backword,	/* ^<- */
	FUNC|META|0x4B,		gotobol,
	FUNC|0x4D,		forwchar,	/* --> */
	FUNC|0x74,		forwword,	/* ^-> */
	FUNC|META|0x4D,		gotoeol,
	FUNC|0x50,		forwline,
	FUNC|0x48,		backline,
     ED|FUNC|0x53,		fbdel,		/* Del */
	CTLX|FUNC|0x53,		togdeldir,
     ED|FUNC|META|0x53,		fbwdel,		/* Esc-Del */
	FUNC|0x49,		backpage,	/* PgUp  */
	FUNC|0x84,		back_nextw,	/* ^PgUp */
	FUNC|0x51,		forwpage,	/* PgDn  */
	FUNC|0x76,		page_nextw,	/* ^PgDn */
	FUNC|0x47,		gotobol,	/* Home  */
	FUNC|0x4F,		gotoeol,	/* End   */
	FUNC|0x77,		gotobob,	/* ^Home    */
	FUNC|0x75,		gotoeob,	/* ^End     */
	FUNC|META|0x47,		reposition,	/* Esc-Home */
	FUNC|META|0x4F,		negrepos,	/* Esc-End  */
     ED|FUNC|0x52,		instog,		/* Insert */
#if	HELP
	FUNC|0x3B,		help,		/* F1  */
#else
	FUNC|0x3B,		listbuffers,
#endif
	FUNC|0x54,		listbuffers,	/* shift-F1 */
	FUNC|0x3C,		splitwind,   	/* F2 */
	FUNC|0x55,		flush_kbuf,
	FUNC|0x3D,		filevisit,	/* F3  */
	FUNC|0x56,		fileread,
	FUNC|0x3E,		onlywind,	/* F4  */
	FUNC|0x57,		filewrite,
	FUNC|0x3F,              forwsearch,     /* F5  */
     ED|FUNC|0x58,		killregion,
	FUNC|0x40,		backsearch,	/* F6  */
     ED|FUNC|0x59,		yank,
	FUNC|0x41,		back_brace,	/* F7  */
	FUNC|0x5A,		forw_brace,
	FUNC|0x42,		quickexit,	/* F8  */
	FUNC|0x5B,		quit,
	FUNC|0x43,		setmark,	/* F9 */
	FUNC|0x5C,		swapmark,
	FUNC|0x44,		ctlxe,		/* F10 */
	FUNC|0x5D,		defmacro,
    /* Alt to act as Ctrl-X-Ctrl: */
	FUNC|0x12,		editog,		/* Alt-E */
     ED|FUNC|0x14,		ltwiddle,	/* Alt-T */
	FUNC|0x2D,		swapmark,	/* Alt-X */
    /* Alt to act as Ctrl-X: */
	FUNC|0x83,		showcpos,
	FUNC|0x78,		onlywind,
	FUNC|0x79,		splitwind,
	FUNC|0x30,		usebuffer,
     ED|FUNC|0x21,		reformat,
	FUNC|0x25,		killbuffer,
     ED|FUNC|0x26,		setlmargin,
	FUNC|0x32,		defmacro,
	FUNC|0x31,		nextwind,
	FUNC|0x19,		prevwind,
     ED|FUNC|0x13,		setfillcol,
	FUNC|0x10,		visitog,
     ED|FUNC|0x1F,		filesave,
	FUNC|0x2F,		filevisit,
    /* Other Alt- functions: */
	FUNC|0x2E,		casestog,	/* Alt-C */
	FUNC|0x20,		spawncli,	/* Alt-D */
	FUNC|0x22,		gotolinum,	/* Alt-G */
     ED|FUNC|0x17,		instog,		/* Alt-I */
     ED|FUNC|0x18,		openline,	/* Alt-O */
     ED|FUNC|0x11,		copyregion,	/* Alt-W */
     ED|FUNC|0x15,		yank,		/* Alt-Y */
	FUNC|0x2C,		spawn,		/* Alt-Z */
	FUNC|0x80,		back_brace,	/* Alt-( */
	FUNC|0x81,		forw_brace,	/* Alt-) */
#if CANLOG
	FUNC|0x7E,		doplay,		/* Alt-7 */
#endif
#if	EXTRA
	FUNC|0x7D,		enlargewind,	/* Alt-^ */
#endif
    /* Still available:
	FUNC|0x23,		Alt-H
	FUNC|0x24,		Alt-J
	FUNC|0x16,		Alt-U
	FUNC|0x7A,		Alt-3
	FUNC|0x7B,		Alt-4
	FUNC|0x7C,		Alt-5
	FUNC|0x7F,		Alt-8
	FUNC|0x82,		Alt '-'
     */
#endif

#if	HELP					/* mb: added */
	META|'?',		help,
	META|CNTL|'[',		help,
#ifdef	HELPCH
	HELPCH,			help,
#endif
#endif

	0,			ctrlg
};

#define NKEYTAB (sizeof(keytab)/sizeof(keytab[0]))

/*
 * Initialize all of the buffers
 * and windows. The file name is passed down as
 * an argument, because the main routine may have been
 * told to read in a file by default, and we want the
 * buffer name to be right.
 */
void edinit(fname)
	char	fname[];
{
	register BUFFER *bp;
	register WINDOW *wp;
	char	bname[NBUFN];

	makename(bname, fname);
	bp = bfind(bname, TRUE, BFEDIT);	/* First buffer		*/
	blistp = bfind("[List]", TRUE, BFTEMP); /* Buffer list buffer	*/
	bhelpp = bfind("[Help]", TRUE, BFTEMP); /* Help screens buffer	*/
	wp = (WINDOW *) malloc(sizeof(WINDOW)); /* First window		*/
	if (bp==NULL || wp==NULL || blistp==NULL || bhelpp==NULL)
#if BFILES
		_exit(1);
#else
		exit(1);
#endif
	curbp  = bp;				/* Make this current	*/
	wheadp = wp;
	curwp  = wp;
	wp->w_wndp  = NULL;			/* Initialize window	*/
	wp->w_bufp  = bp;
	bp->b_nwnd  = 1;			/* Displayed.		*/
	wp->w_linep = bp->b_linep;
	wp->w_dotp  = bp->b_linep;
	wp->w_doto  = 0;
	wp->w_markp = NULL;
	wp->w_marko = 0;
	wp->w_toprow = 0;
	wp->w_ntrows = term.t_nrow-1;		/* "-1" for mode line.  */
	wp->w_force = 0;
	wp->w_offset = 0;
	wp->w_flag  = WFMODE|WFHARD;		/* Full.		*/
}

/*
 * This is the general command execution
 * routine. It handles the fake binding of all the
 * keys to "self-insert". It also clears out the "thisflag"
 * word, and arranges to move it to the "lastflag", so that
 * the next command can look at it. Return the status of
 * command.
 * mb: added the BFEDIT / ED stuff.
 */
int
execute(c, f, n)
	register int	c;
  int f, n;
{
	register KEYTAB *ktp;
	register int	k;
	register int	d;
	register int	status;

	if ((c & 0xFF00) == 0)
		goto ascii;
	d = c;
	if (curbp->b_flag & BFEDIT)
		d |= ED;
	ktp = &keytab[0];			/* Look in key table.	*/
	while (ktp < &keytab[NKEYTAB]) {
		k = ktp->k_code;
		if (k==c || k==d) {		/* mb: fit, ED| or not */
			thisflag = 0;
			status   = (*ktp->k_fp)(f, n);
			lastflag = thisflag;
			return (status);
		}
		++ktp;
	}
	if (c != d) {
#if GDEBUG
		mlwrite("No such command (code 0x%x)", c);
#else
		mlwrite("No such command");
#endif
		return (FALSE);
	} /* else fall thru to "view-only" message */
ascii:
	if (! (curbp->b_flag & BFEDIT)) {
#if (AtST | MSDOS | W32)
		mlwrite("View-only mode - Alt-E to edit");
#else
		mlwrite("View-only mode - ^X^E to edit");
#endif
		lastflag = 0;			/* Fake last flags.	*/
		return(FALSE);
	}

	/* mb:
	 * If fill column is defined, the argument is positive,
	 * and we are now past fill column, perform word wrap.
	 * Don't insert a space if just wrapped.
	 */
	if (fillcol>0 && n>0 && getccol(FALSE)>=fillcol) {
		wrapword(c);
		if (c == ' ') {
			lastflag = 0;
			return (FALSE);
		}
	}
					/* mb: skipped the ASCII check	*/
	if (n <= 0) {				/* Fenceposts.		*/
		lastflag = 0;
		return (n<0 ? FALSE : TRUE);
	}
	thisflag = 0;				/* For the future.	*/
	status   = linsert(n, c, ovrstrk);	/* mb: added param	*/
	lastflag = thisflag;
	return (status);
}

#if (ST_DA == 0)
void usage()
{
#if AtST
	Cconws("\r\nMEX version " VERSION "\r\n");
	Cconws("\r\nUsage: mex [options] [file(s)]\r\n");
	Cconws("\r\nOptions:\r\n");
	Cconws("\t-c #\t# columns\r\n");
	Cconws("\t-f #\tfill column = #\r\n");
	Cconws("\t-f #-#\tleft & right margins\r\n");
	Cconws("\t-g #\tgoto line #\r\n");
	Cconws("\t-i\tinverse video\r\n");
#if CANLOG
#if LOGIT
	Cconws("\t-l\tdon't log keystrokes\r\n");
#else
	Cconws("\t-l\tlog keystrokes\r\n");
#endif
	Cconws("\t-p\tplay back log file\r\n");
#endif
	Cconws("\t-r #\t# rows\r\n");
	Cconws("\t-t #\ttabsize = #\r\n");
	Cconws("\t-v\tview-only mode\r\n\n");
	Cconws("\tPress any key ");
	Cnecin();
	_exit(0);
#endif
#if (MSDOS)
	cputs("\r\nMEX version " VERSION "\r\n");
	cputs("\r\nUsage: mex [options] [file(s)]\r\n");
	cputs("\r\nOptions:\r\n");
	cputs("-c #    # columns\r\n");
	cputs("-d      BIOS, rather than direct, screen output\r\n");
	cputs("-f #    fill column = #\r\n");
	cputs("-f #-#  left & right margins\r\n");
	cputs("-g #    goto line #\r\n");
	cputs("-i      inverse video\r\n");
#if CANLOG
#if LOGIT
	cputs("-l      don't log keystrokes\r\n");
#else
	cputs("-l      log keystrokes\r\n");
#endif
	cputs("-m #    text mode #\r\n");
	cputs("        (0=BW40, 1=C40, 2=BW80, 3=C80, 7=MONO)\r\n");
	cputs("-p      play back log file\r\n");
#endif
	cputs("-r #    # rows\r\n");
	cputs("-t #    tabsize = #\r\n");
	cputs("-v      view-only mode\r\n\n");
	_exit(0);
#endif
#if (V7 | VMS | CPM | W32)
	puts("\nMEX version " VERSION);
	puts("\nUsage: mex [options] [file(s)]");
	puts("Options:");
	puts("\t-c #\t# columns");
	puts("\t-f #\tfill column = #");
	puts("\t-f #-#\tleft & right margins");
	puts("\t-g #\tgoto line #");
#if CANLOG
#if LOGIT
	puts("\t-l\tdon't log keystrokes");
#else
	puts("\t-l\tlog keystrokes");
#endif
	puts("\t-p\tplay back log file");
#endif
	puts("\t-r #\t# rows");
	puts("\t-t #\ttabsize = #");
	puts("\t-v\tview-only mode");
#if V7
#if CANLOG
	puts("\nSuggested: setenv MEXLOG $HOME/.mexlog");
#endif
#endif
	exit(0);
#endif
}
#endif

#if ST_DA
#define register		/* avoid that in main()! */
#endif

#if VT100 && !W32
int
escseq(c)
	register int c;
{ 
	if (c=='[' || c=='O') {		/* Arrows and extras.	*/
		c = getkey();
		if (c == 'A')
			return (CNTL | 'P');
		if (c == 'B')
			return (CNTL | 'N');
		if (c == 'C')
			return (CNTL | 'F');
		if (c == 'D')
			return (CNTL | 'B');

		if (c == 'L')
			return (META | 'I');
		if (c == 'H')
			return (CNTL | 'A');
		if (c == 'F')
			return (CNTL | 'E');
		if (c == 'I')
			return (META | 'V');
		if (c == 'G')
			return (CNTL | 'V');

		if (c == 'P')	/* PF1 */
			return (META | 'B');
		if (c == 'Q')	/* PF2 */
			return (META | 'F');
		if (c == 'R')	/* PF3 */
			return (META | '.');
		if (c == 'S')	/* PF4 */
			return (CTLX | 'E');
		/* else */
			return (0);
	}
	return (0);
}
#endif

#if MSDOS
int _main(argc, argv)
#else
int main(argc, argv)
#endif
	int	argc;
	char	*argv[];
{
	register int	c=0;
	register int	f;
	register int	n;
	register int	state;
	int	negarg;
	int	gline = 1;
	int	visitmode = FALSE;
	char	*cp;
#if MSDOS
	directvideo = TRUE;
	_fmode = O_BINARY;
#endif
	nfiles = 0;
#if ST_DA
	maxnfiles = NFILES;
#else
	maxnfiles = argc + NFILES;
	if ((clfn = (char **) malloc (maxnfiles * sizeof(char *))) == NULL)
#if BFILES
		_exit (1);
#else
		exit (1);
#endif
	while(--argc > 0 && ++argv != NULL) {
		cp = *argv;
		if(*cp == '-') {	/* cmd line parameter */
			f = *(++cp);
			if (f >= 'a' && f <= 'z')
				f += ('A' - 'a');
			if (f && cp[1]=='\0'	/* space before number   */
			 && argc > 1 && argv[1] != NULL
			 && (c=argv[1][0])>='0' && c<='9') {
				if (f=='C' || f=='F' || f=='G'
				 || f=='R' || f=='T') {
					--argc;
					++argv;
					cp = (*argv) - 1;
				}
			}
			n = 0;
			while (f && (c=(*(++cp)))>='0' && c<='9')
				n = 10*n + (c-'0');
			if (f == 'C')
				term.t_ncol = n;
			else if (f == 'F') {
				if (c == '-') {
					lmargin = n;
					n = 0;
					while (f &&
						(c=(*(++cp)))>='0' && c<='9')
							n = 10*n + (c-'0');
				}
				fillcol = n;
				if (lmargin + tabsize > fillcol)
					lmargin = fillcol = 0;
			}
#if MSDOS
			else if (f == 'D')
				directvideo = FALSE;
#endif
			else if (f == 'G')
				gline = n;
#if (AtST | MSDOS)
			else if (f == 'I')
				vidrev = TRUE;
#endif
#if CANLOG
			else if (f == 'L')
				logit = (!logit);
			else if (f == 'P')
				playback = TRUE;
#endif
#if MSDOS
			else if (f == 'M') {
				if ((n>=0 && n<=3) || n==7)
					vidmode = n;
			}
#endif
			else if (f == 'R')
				term.t_nrow = n-1;
			else if (f == 'T')
				tabsize = n;
			else if (f == 'V')
				visitmode = TRUE;
			else
				usage();
		} else {			/* a filename	*/
			clfn[nfiles++] = cp;
#if (AtST | MSDOS | CPM | W32)
			cpyfname (cp, cp);	/* tolower	*/
#endif
		}
	}
#endif
#if AtST
	if (Getrez() == 0) {
		if (term.t_ncol > 40)
			term.t_ncol = 40;
	}
#endif
#if MSDOS
	if (vidmode==0 || vidmode==1) {
		if (term.t_ncol > 40)
			term.t_ncol = 40;
	}
#endif
	vtinit();
	if (nfiles) {
		cp = clfn[0];
		edinit(cp);
		cpyfname (curbp->b_fname, cp);
		update(TRUE);
		if (readin(cp) == FIOFNF)
			mlwrite("[New file]");
		if (visitmode) {
			curbp->b_flag &= (~BFEDIT);
			logit = FALSE;
		}
		for (fileindex = 1; fileindex<nfiles; fileindex++) {
			edmore(clfn[fileindex]);
		}
	} else {				/* no filename given */
		edinit("main");
		fileindex = 0;
	}
	gotolinum(TRUE,gline);
	kbdm[0] = CTLX|')';			/* Empty macro		*/
	lastflag = 0;				/* Fake last flags.	*/
	f = FALSE;
	n = 1;
	negarg = FALSE;
#if ST_DA
	state = DASTART;
	c = ~NOKEY;
#else
	update(TRUE);
	c = getkey();
#if (V7 | VMS)
	while (c==(CNTL|'Q') || c==(CNTL|'S'))
		c = getkey();			/* mb: dump handshakes	*/
#endif
	if (c != NOKEY)
		mlerase();
	state = BASE;
#endif

	for(;;) {				/* main loop */

	while (c == NOKEY) {	/* mb: happens after errors in getkey() */
		update(FALSE);
		c = getkey();
	}

	switch (state) {

	case BASE:
		if (c==(CNTL|'U')) {
			n = 4;
			mlwrite("arg: 4");
			c = getkey();
			state = ARG;
			break;
		}
		if (c == METACH) {
			c = getkey();
			state = ESC;
			break;
		}
#if W32
		if (c == 0xE0) {
			c = getkey();
			state = ESC;
			break;
		}
#endif
		if (c == (CNTL|'X')) {
			c = getkey();
#if AtST
			if (c==(FUNC|0x61) || c==(CNTL|'G')) { /* Undo */
#endif
#if (MSDOS | W32)
			if (c==(FUNC|0x3C) || c==(CNTL|'G')) { /* F2 */
#endif
#if (V7 | VMS | CPM)
			if (c == (CNTL|'G')) {
#endif
				c = getkey();
				break;
			}
			if (c>='a' && c<='z')
				c -= 0x20;	/* force upper case */
			c |= CTLX;
		}
		state = EXEC;
		break;

	case ESC:
		if (c==('#')) {
			n = 4;
			mlwrite("arg: 4");
			c = getkey();
			state = ARG;
			break;
		}
		if (c=='-' || (c>='0' && c<='9')) {
			state = ARG;
			break;
		}
#if VT100 && !W32
		if (c=='[' || c=='O') {
			state = EXEC;
			c = escseq(c);
			if (! c) {
				c = getkey();
				state = BASE;
			}
			break;
		}
#endif
#if (HP700)
		if (c == 'A') {
			c = (CNTL | 'P');
			state = BASE;
			break;
			}
		if (c == 'B') {
			c = (CNTL | 'N');
			state = BASE;
			break;
			}
		if (c == 'C') {
			c = (CNTL | 'F');
			state = BASE;
			break;
			}
		if (c == 'D') {
			c = (CNTL | 'B');
			state = BASE;
			break;
			}
		if (c == 'S') {
			c = (META | 'R');
			state = BASE;
			break;
			}
		if (c == 'T') {
			c = (META | 'S');
			state = BASE;
			break;
			}
#endif
#if (W32)
		if (c == 'H') {
			c = (CNTL | 'P');
			state = BASE;
			break;
			}
		if (c == 'P') {
			c = (CNTL | 'N');
			state = BASE;
			break;
			}
		if (c == 'K') {
			c = (CNTL | 'B');
			state = BASE;
			break;
			}
		if (c == 'M') {
			c = (CNTL | 'F');
			state = BASE;
			break;
			}
		if (c == 'Q') {
			c = (CNTL | 'V');
			state = BASE;
			break;
			}
		if (c == 'I') {
			c = (META | 'V');
			state = BASE;
			break;
			}
		if (c == 'G') {
			c = (CNTL | 'A');
			state = BASE;
			break;
			}
		if (c == 'O') {
			c = (CNTL | 'E');
			state = BASE;
			break;
			}
		if (c == 'S') {
			c = (CNTL | 'D');
			state = BASE;
			break;
			}
		if (c == 'R') {
			c = (ED | FUNC | 0x52);
			state = BASE;
			break;
			}
#endif
#if AtST
		if (c==(FUNC|0x61) || c==(CNTL|'G')) {	/* Undo */
#endif
#if (MSDOS | W32)
		if (c==(FUNC|0x3C) || c==(CNTL|'G')) {  /* F2 */
#endif
#if (V7 | VMS | CPM)
		if (c == (CNTL|'G')) {
#endif
			c = getkey();
			state = BASE;
			break;
		}
		if (c>='a' && c<='z')
			c -= 0x20;		/* force upper case */
		c |= META;
		state = EXEC;
		break;

	case ARG:
		if (c == (CNTL|'U')) {
			n *= 4;
		} else if (c=='-' && f==FALSE) {
			negarg = TRUE;
			n = (-1);
		} else if  (c==(CNTL|'H')  || c==(CNTL|0x3F)
#if AtST
			 || c==(FUNC|0x0E) || c==(FUNC|0x53)
#endif
#if MSDOS
			 || c==(FUNC|0x53)
#endif
							) {
			n /= 10;
			if (n == 0)
				negarg = FALSE;

		} else if (c == (CNTL|'G') || c==(CNTL|'C')
#if AtST
			|| c==(FUNC|0x61)	/* Undo */
#endif
#if MSDOS
			|| c==(FUNC|0x3C)	/* F2 */
#endif
								) {
			f = FALSE;
			n = 1;
			mlwrite("[aborted]");
			update(FALSE);		/* put cursor back */
			c = getkey();
			state = BASE;
			break;
		} else if (c>='0' && c<='9') {
			if (f == FALSE) {
				n = 0;
				f = TRUE;
			}
			n = 10*n + ((negarg) ? ('0'-c) : (c-'0'));
		} else {
			mlwrite("[numeric prefix used]");
			state = BASE;
			f = TRUE;
			break;
		}
		mlwrite("arg: %d",n);
		c = getkey();
		break;

	case EXEC:
#if V7
		if (c == (META|'Q')) {
			c = getkey();
			if (c & CNTL)
				c ^= 0x40;
			c &= 0xFF;
		}
#else
		if (c == (CNTL|'Q') || c == (META|'Q')) {
			c = getkey();
			if (c & CNTL)
				c ^= 0x40;
			c &= 0xFF;
		}
#endif
		if (kbdmip != NULL) {		/* Save macro strokes.  */
			if (c!=(CTLX|')') && kbdmip>&kbdm[NKBDM-6]) {
				ctrlg();
				c = getkey();
				state = BASE;
				break;
			}
			if (f != FALSE) {
				*kbdmip++ = (CNTL|'U');
				*kbdmip++ = n;
			}
			*kbdmip++ = c;
		}
#if (VT100 || HP700)
		if (c==127) c = CNTL|'D';
		if (c==(CNTL|0x3F)) c = CNTL|'H';
#endif
		f = execute(c, f, n);		/* Do it, finally */
#if ST_DA
		if (f == DACLOSE) {		/* cmd was quit() */
			state = DACLOSE;
			break;
		}
#endif
		f = FALSE;
		n = 1;
		negarg = FALSE;
		update(FALSE);			/* Fix up the screen	*/
		c = getkey();
		if (c != NOKEY && mpresf != FALSE) {
			mlerase();
		}
		state = BASE;
		break;

#if ST_DA

	case DASTART:

		Cconws ("\033f");	/* disable vt52 cursor	*/
		appl_init();
		menu_id = menu_register (gl_apid,"  MEX");
		wind_get (0, WF_WORKXYWH, &xdesk, &ydesk, &wdesk, &hdesk);
daloop:
		evnt_mesag (msgbuff);
		if ( msgbuff[0] != AC_OPEN
		  || msgbuff[4] != menu_id )
				goto daloop;
		wind_update(TRUE);
		graf_mouse(M_OFF,0x0L);
		cp = (char *) Physbase();
		for (c=0; c<1520; c++)
			menusave[c] = *cp++;
		form_dial (FMD_START,
				xdesk, ydesk, wdesk, hdesk,
				xdesk, ydesk, wdesk, hdesk);
		(*term.t_open)();
		sgarbf = TRUE;
		c = NOKEY;
		state = BASE;
		break;

	case DACLOSE:

		(*term.t_close)();
		form_dial (FMD_FINISH,
				xdesk, ydesk, wdesk, hdesk,
				xdesk, ydesk, wdesk, hdesk);
		cp = (char *) Physbase();
		for (c=0; c<1520; c++)
			*cp++ = menusave[c];
		graf_mouse(M_ON,0x0L);
		wind_update (FALSE);
		goto daloop;
#endif

	}					/* end of switch */

	}					/* end of for()  */

}						/* end of main() */

#ifdef register
#undef register
#endif

/*
 *  mb: Display another file in another window.
 *      Called only if a second filename appears in command line.
 */
void
edmore(fname)
	char	fname[];
{
	register BUFFER *bp;
	char	bname[NBUFN];

	makename(bname, fname);
	if (bfind(bname, FALSE, 0))	/* buffer with that name exists */
		strcpy (bname, "[same as another]");
	bp = bfind(bname, TRUE, 0);	/* create second buffer 	*/
	if ( bp == NULL
	  || splitwind(0,1) != TRUE	/* display it in a 2nd window	*/
	  || nextwind(0,1)  != TRUE)
		return;
	gotobuf(bp);
	cpyfname (bp->b_fname, fname);
	if (readin(fname) == FIOEOF) {
		curwp->w_flag |= WFMODE|WFFORCE|WFHARD;
		mlwrite("[more files]");
	} else
		mlwrite("Error reading next file!");
	prevwind(0,1);
}

/*
 * Read in a key.
 * Convert ctrl keys to the internal character set.
 * mb: simplified, but added the log file stuff.
 */
int
getkey() {
	register int c, ub, lb=0;

#if CANLOG
	if (playback == TRUE) {
		if (ropenlog() == ABORT) {
			playback = ABORT;
			(*term.t_beep)();
			mlwrite("Unable to open log file");
			return (NOKEY);
		}
		ub = getlog();
		if (ub != EOF)
			lb = getlog();
		if (ub==EOF || lb==EOF) {
			closelogf();
			playback = ABORT;
			mlwrite("[playback done]");
			return (NOKEY);
		}
		c = lb & 0xFF;
		c |= (ub & 0xFF) << 8;
		return (c);
	}

	if (!logit || playback==ABORT)
		goto logok;		/* don't log after playback */

	if (wopenlog() == ABORT) {
		playback = ABORT;
		(*term.t_beep)();
		mlwrite("Unable to open log file");
		return (NOKEY);
	}
#endif

logok:

	c = (*term.t_getchar)();
#ifdef CNTLCH
	if (c == CNTLCH) {			/* Apply C- prefix	*/
		c = (*term.t_getchar)();
		if (c>='a' && c<='z')		/* Force to upper	*/
			c -= 0x20;
		c |= CNTL;
	}
#endif
#if AtST
	if (c == (FUNC|SHFT|0x1E)) {		/* Alt-A */
		c = (*term.t_getchar)() & 0xFF;
		c |= 0x80;		/* set MSB: alternate char set */
	}
#endif
#if MSDOS
	if (c == (FUNC|0x1E)) {			/* Alt-A */
		c = (*term.t_getchar)() & 0xFF;
		c |= 0x80;		/* set MSB: alternate char set */
	}
#endif
#if CURSES
	switch (c)
		{
		case KEY_BACKSPACE:		/* backspace */
					c = (CNTL | 'H');		break;
		case KEY_NEXT:			/* tabulator */
					c = (CNTL | 'I');		break;
		case KEY_UP:			/* up arrow */
					c = (CNTL | 'P');		break;
		case KEY_DOWN:			/* down arrow */
					c = (CNTL | 'N');		break;
		case KEY_RIGHT:		/* right arrow */
					c = (CNTL | 'F');		break;
		case KEY_LEFT:			/* left arrow */
					c = (CNTL | 'B');		break;
		case KEY_PPAGE:		/* PGUP key */
					c = (META | 'V');     		break;
		case KEY_NPAGE:		/* PGDN key */
					c = (CNTL | 'V');     		break;
		case KEY_LL:			/* END key */
#ifdef KEY_END
		case KEY_END:			/* END key */
#endif
					c = (CNTL | 'E');  		break;
		case KEY_HOME:			/* home key */
					c = (CNTL | 'A');		break;
		case KEY_IC:			/* insert/overstrike toggle */
					c = (META | 'I');		break;
		case KEY_DC:			/* delete key */
					c = (CNTL | 'D');		break;
		case KEY_F(1):			/* F1 key, help */
					c = (META | '?');		break;
		case KEY_F(2):			/* F2 key, new window */
					c = (CTLX | '2');		break;
		case KEY_F(3):			/* F3 key, new file */
					c = (CTLX | CNTL | 'V');	break;
		case KEY_F(4):			/* F4 one window */
					c = (CTLX | '1');		break;
		case KEY_F(5):			/* F5 search forward */
					c = (CNTL | 'S');		break;
		case KEY_F(6):			/* F6 search backward */
					c = (CNTL | 'R');		break;
		case KEY_F(7):			/* F7 search bracket */
					c = (META | '{'); 		break;
		case KEY_F(8):			/* F8 save/exit */
					c = (CNTL | 'Z');		break;
		case KEY_F(9):			/* F9 save/exit */
					c = (CNTL | 'Z');		break;
		case KEY_F(10):			/* F10 save/exit */
					c = (CNTL | 'Z');		break;
		}
#endif
	/* control -> CNTL */
	if ((!(FUNC&c)) && (c<0x20 || c==0x7F))
		c = CNTL | (c ^ 0x40);
#if CANLOG
	if (!logit || playback==ABORT)
		goto nolog;		/* don't log after playback */

	lb = c & 0xFF;
	ub = (c>>8) & 0xFF;
	ub = putlog (ub);
	lb = putlog (lb);
	if (ub==EOF || lb==EOF) {
		closelogf();
		playback = ABORT;
		(*term.t_beep)();
		mlwrite("Error writing log file");
		return (NOKEY);
	}
	flushlog(FALSE);		/* flushes only when full */
#endif
nolog:
	return (c);
}

#if CANLOG
/*
 * mb: turn on playback from inside the editor.
 */
int
doplay(f,n)
	int f, n;
{
	register int c, s;
	char buf[80];

	if (playback == TRUE)
		return (FALSE);
	s = mlreply("Playback? (y/n)", "no", buf, 80, FALSE);
	if (s==TRUE && ((c=buf[0])=='y' || c=='Y')) {
		if (logit == TRUE) {
			flushlog(TRUE);
			closelogf();
		}
		logit = FALSE;
		playback = TRUE;
		return (TRUE);
	}
	return (ctrlg());
}
#endif

/*
 * Fancy quit command, as implemented
 * by Norm. If the current buffer has changed
 * do a write current buffer and exit emacs,
 * otherwise simply exit.
 */
int
quickexit(f,n)
	int f, n;
{
	if ((curbp->b_flag&BFCHG) != 0		/* Changed.		*/
	&& (curbp->b_flag&BFTEMP) == 0)		/* Real.		*/
		if (filesave(f, n) != TRUE)
			return (FALSE);
	return (quit(f, n));			/* conditionally quit	*/
}

/*
 * Quit command. If an argument, always
 * quit. Otherwise confirm if a buffer has been
 * changed and not written out. Normally bound
 * to "C-X C-C" now "C-C".
 */
int
quit(f, n)
	int f, n;
{
	register int	s;
#if ST_DA
	char	*msg = "Suspend unsaved buffer(s)";
#else
	char	*msg = "Discard changes";
#endif
#if CANLOG
	if (playback == TRUE)
		return (FALSE);
#endif
	if (anycb() == FALSE			/* All buffers clean.	*/
	|| (s=mlyesno(msg)) == TRUE) {		/* User says OK.	*/
#if CANLOG
		if (logit == TRUE) {
			flushlog(TRUE);
			closelogf();
		}
#endif
		vttidy();
#if ST_DA
		return (DACLOSE);
#else
#if BFILES
		_exit(GOOD);
#else
		exit(GOOD);
#endif
#endif
	}
	mlwrite("[aborted]");
	return (s);
}

/* mb: added */
int
emacs_quit(f, n)
	int f, n;
{
	if (f)
		return(quickexit(FALSE, 1));
	else
		return(quit(f, n));
}

/*
 * Begin a keyboard macro.
 * Error if not at the top level
 * in keyboard processing. Set up
 * variables and return.
 */
int
ctlxlp(f, n)
	int f, n;
{
	if (kbdmip!=NULL || kbdmop!=NULL) {
		mlwrite("Not now");
		return (FALSE);
	}
	mlwrite("[Start macro]");
	kbdmip = &kbdm[0];
	return (TRUE);
}

/*
 * End keyboard macro. Check for
 * the same limit conditions as the
 * above routine. Set up the variables
 * and return to the caller.
 */
int
ctlxrp(f, n)
	int f, n;
{
	if (kbdmip == NULL) {
		mlwrite("Not now");
		return (FALSE);
	}
	*(--kbdmip) = CTLX|')';		/* in case CTLX|'M' */
	mlwrite("[End macro]");
	kbdmip = NULL;
	return (TRUE);
}

/* mb: added.
 */
int
defmacro(f, n)
	int f, n;
{
	if (kbdmip == NULL)
		return (ctlxlp(f, n));
	else
		return (ctlxrp(f, n));
}

/*
 * Execute a macro.
 * The command argument is the
 * number of times to loop. Quit as
 * soon as a command gets an error.
 * Return TRUE if all ok, else
 * FALSE.
 */
int
ctlxe(f, n)
	int f, n;
{
	register int	c;
	register int	af;
	register int	an;
	register int	s;

	if (kbdmip!=NULL || kbdmop!=NULL) {
		mlwrite("Not now");
		return (FALSE);
	}
	if (n <= 0) 
		return (TRUE);
	do {
		kbdmop = &kbdm[0];
		do {
			af = FALSE;
			an = 1;
			if ((c = *kbdmop++) == (CNTL|'U')) {
				af = TRUE;
				an = *kbdmop++;
				c  = *kbdmop++;
			}
			s = TRUE;
		} while (c!=(CTLX|')') && (s=execute(c, af, an))==TRUE);
		kbdmop = NULL;
	} while (s==TRUE && --n);
	return (s);
}

/*
 * Abort.  Kill off any keyboard macro, etc., that is in progress.
 * Sometimes called as a routine, to do general aborting of stuff.
 */
int
ctrlg()
{
	mlwrite("[aborted]");		/* mb: instead of beep */
	if (kbdmip != NULL) {
		kbdm[0] = (CTLX|')');
		kbdmip  = NULL;
	}
	return (ABORT);
}

#ifdef nonono
  CNTL|'@',   setmark,         //  0
  CNTL|'A',   gotobol,         //  1 KEY_HOME:        /* HOME key */       c = (CNTL | 'A')
  CNTL|'B',   backchar,        //  2 KEY_LEFT:        /* left arrow */     c = (CNTL | 'B')
  CNTL|'C',   quit,            //  3
  CNTL|'D',   forwdel,         //  4 KEY_DC:          /* delete key */     c = (CNTL | 'D')
  CNTL|'E',   gotoeol,         //  5 KEY_END:         /* END key */        c = (CNTL | 'E')
  CNTL|'F',   forwchar,        //  6 KEY_RIGHT:       /* right arrow */    c = (CNTL | 'F')
  CNTL|'G',   undo,            //  7
  CNTL|'H',   backdel,         //  8 KEY_BACKSPACE:   /* backspace */      c = (CNTL | 'H')
  CNTL|'I',   tab,             //  9 KEY_NEXT:        /* tabulator */      c = (CNTL | 'I')
  CNTL|'J',   tnewline,        // 10
  CNTL|'K',   killtxt,         // 11
  CNTL|'L',   refresh,         // 12
  CNTL|'M',   tnewline,        // 13
  CNTL|'N',   forwline,        // 14 KEY_DOWN:        /* down arrow */     c = (CNTL | 'N')
  CNTL|'O',   openline/IC!,    // 15 KEY_IC:          /* Ins toggle */     c = (CNTL | 'O')
  CNTL|'P',   backline,        // 16 KEY_UP:          /* up arrow */       c = (CNTL | 'P')
  CNTL|'Q',   TERMINAL START   // 17
  CNTL|'R',   backpage,        // 19 KEY_PPAGE:       /* PGUP key */       c = (CNTL | 'V')
  CNTL|'S',   TERMINAL STOP    // 18
  CNTL|'T',   twiddle,         // 20
  CNTL|'U',   MULTIPLE,        // 21
  CNTL|'V',   forwpage,        // 22 KEY_NPAGE:       /* PGDN key */       c = (CNTL | 'V')
  CNTL|'W',   killregion,      // 23
  CNTL|'X',   META2,           // 24
  CNTL|'Y',   yank,            // 25
  CNTL|'Z',   quickexit,       // 26 KEY_F(8):        /* F8 save/exit */   c = (CNTL | 'Z')
  ESC         META,            // 27

KEY_IC:          /* Ins toggle */     c = (META | 'I')
KEY_F(1):        /* F1 help */        c = (META | '?')
KEY_F(2):        /* F2 new window */  c = (CTLX | '2')
KEY_F(3):        /* F3 new file */    c = (CTLX | CNTL | 'V')
KEY_F(4):        /* F4 one window */  c = (CTLX | '1')
KEY_F(5):        /* F5 search forw */ c = (META | 'S')
KEY_F(6):        /* F6 search back */ c = (META | 'R')
KEY_F(7):        /* F7 sea bracket */ c = (META | '{')
KEY_F(9):        /* F9 save/exit */   c = (CNTL | 'Z')
KEY_F(10):       /* F10 save/exit */  c = (CNTL | 'Z')
#endif
