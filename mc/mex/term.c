/* termio.c
 * The functions in this file
 * negotiate with the operating system
 * for characters, and write characters in
 * a barely buffered fashion on the display.
 * All operating systems.
 */

#define TERMC 1
#include	"ed.h"

#if W32
#include <windows.h>
#include <stdio.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif
static HANDLE stdoutHandle;
static DWORD outModeInit;
#endif

#if AtST
extern int vidrev;
#endif

#if	VMS
#include	<stdio.h>
#include	<stsdef.h>
#include	<ssdef.h>
#include	<descrip.h>
#include	<iodef.h>
#include	<ttdef.h>

#define	NIBUF	128			/* Input  buffer size		*/
#define	NOBUF	1024			/* MM says bug buffers win!	*/
#define	EFN	0			/* Event flag			*/

char	obuf[NOBUF];			/* Output buffer		*/
int	nobuf;				/* # of bytes in above		*/
char	ibuf[NIBUF];			/* Input buffer			*/
int	nibuf;				/* # of bytes in above		*/
int	ibufi;				/* Read index			*/
int	oldmode[2];			/* Old TTY mode bits		*/
int	newmode[2];			/* New TTY mode bits		*/
short	iochan;				/* TTY I/O channel		*/
#endif

#if	CPM
#include	<bdos.h>
#endif

#if W32
#include	<conio.h>
#endif

#if	MSDOS
#include	<bios.h>
#include	<dos.h>
#include	<conio.h>
extern int origvidmode, nrmlvidattr, curvidattr, vidrev, vidmode, directvid;
void ttinverse()
{
	int fore, back, attr;
	attr = nrmlvidattr;
	fore = attr & 0x07;			/* foreground	*/
	back = attr & 0x70;			/* background	*/
	attr = (fore << 4) | (back >> 4);	/* swap		*/
	textattr (attr | 0x00);			/* hi-lite fore with 0x08 */
	curvidattr = attr;
}
void ttnormal()
{
	textattr (nrmlvidattr | 0x00);		/* hi-lite fore with 0x08 */
	curvidattr = nrmlvidattr;
}
void ttreverse()
{
	if (curvidattr == nrmlvidattr)
		ttinverse();
	else
		ttnormal();
}
#endif

#if V7
#include	<stdio.h>
#if !SYS_V
#include	<sgtty.h>		/* for stty/gtty functions */
struct  sgttyb  ostate;			/* saved tty state */
struct	sgttyb	nstate;			/* values for editor mode */
#else
#if BSD
#include <termios.h>
struct  termios  ostate;
struct  termios  nstate;
#else
#include <termio.h>
struct  termio  ostate;
struct  termio  nstate;
#endif
#endif

#if	CURSES
#ifdef hpux
	static int hpterm = FALSE;
#endif
#endif
#endif

/*
 * This function is called once
 * to set up the terminal device streams.
 * On VMS, it translates SYS$INPUT until it
 * finds the terminal, then assigns a channel to it
 * and sets it raw. On CPM it is a no-op.
 */
int ttopen()
{
#if	VMS
	struct	dsc$descriptor	idsc;
	struct	dsc$descriptor	odsc;
	char	oname[40];
	int	iosb[2];
	int	status;

	odsc.dsc$a_pointer = "SYS$INPUT";
	odsc.dsc$w_length  = strlen(odsc.dsc$a_pointer);
	odsc.dsc$b_dtype   = DSC$K_DTYPE_T;
	odsc.dsc$b_class   = DSC$K_CLASS_S;
	idsc.dsc$b_dtype   = DSC$K_DTYPE_T;
	idsc.dsc$b_class   = DSC$K_CLASS_S;
	do {
		idsc.dsc$a_pointer = odsc.dsc$a_pointer;
		idsc.dsc$w_length  = odsc.dsc$w_length;
		odsc.dsc$a_pointer = &oname[0];
		odsc.dsc$w_length  = sizeof(oname);
		status = LIB$SYS_TRNLOG(&idsc, &odsc.dsc$w_length, &odsc);
		if (status!=SS$_NORMAL && status!=SS$_NOTRAN)
			exit(status);
		if (oname[0] == 0x1B) {
			odsc.dsc$a_pointer += 4;
			odsc.dsc$w_length  -= 4;
		}
	} while (status == SS$_NORMAL);
	status = SYS$ASSIGN(&odsc, &iochan, 0, 0);
	if (status != SS$_NORMAL)
		exit(status);
	status = SYS$QIOW(EFN, iochan, IO$_SENSEMODE, iosb, 0, 0,
			  oldmode, sizeof(oldmode), 0, 0, 0, 0);
	if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
		exit(status);
	newmode[0] = oldmode[0];
	newmode[1] = oldmode[1] | TT$M_PASSALL | TT$M_NOECHO;
	status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
			  newmode, sizeof(newmode), 0, 0, 0, 0);
	if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
		exit(status);
#endif
#if	CPM
#endif
#if VT100
term.t_ncol = (getenv("COLUMNS") ? atoi(getenv("COLUMNS")) : NCOL);
term.t_nrow = (getenv("LINES")   ? atoi(getenv("LINES"))   : NROW) - 1;
#endif
#if W32
DWORD outMode = 0;
CONSOLE_SCREEN_BUFFER_INFO csbi;
stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
GetConsoleMode(stdoutHandle, &outMode);
outModeInit = outMode;
outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
SetConsoleMode(stdoutHandle, outMode);
GetConsoleScreenBufferInfo(stdoutHandle, &csbi);
term.t_ncol = csbi.srWindow.Right - csbi.srWindow.Left + 1;
term.t_nrow = csbi.srWindow.Bottom - csbi.srWindow.Top;
#endif
#if	MSDOS
	struct text_info tinfo;
	putch(' ');			/* get orig attr for normvideo() */
	gettextinfo (&tinfo);
	origvidmode = tinfo.currmode;
	if (vidmode >= 0)
		textmode (vidmode);
	gettextinfo (&tinfo);
	nrmlvidattr = curvidattr = tinfo.attribute;
	if (vidrev)
		ttinverse();
#endif
#if	AtST
	if (vidrev) {
		/* set background color to black, foreground to white */
		ttputs ("\033co\033b0");
	} else {
		/* set background color to white, foreground to black */
		ttputs ("\033c0\033bo");
	}
	vt52nrml();
	ttputs ("\033e");		/* enable text cursor */
#endif
#if     V7
#if     !SYS_V
	ioctl(1,TIOCFLUSH,0);		/* mb: wait for chars to flush	*/
#if SLEEP
	sleep(SLEEP);			/* still seems necessary!	*/
#endif
	gtty(1, &ostate);			/* save old state */
	gtty(1, &nstate);			/* get base of new state */
	nstate.sg_flags |= RAW;
	nstate.sg_flags &= ~(ECHO|CRMOD);	/* no echo for now... */
	stty(1, &nstate);			/* set mode */
#else
#if	CURSES
	WINDOW *status;
	struct termios t;

	status = initscr();
#ifdef init_tabs
	init_tabs = 0;
#endif
	if (status==NULL) {
		fprintf(stderr, "Terminfo setup failed\n");
		exit(1);
	}
#ifdef hpux
	if (strstr(getenv("TERM"), "hp")) hpterm = TRUE;
#endif
	term.t_ncol = columns;
	term.t_nrow = lines - 1;

	tcgetattr (fileno(stdin), &t);
	t.c_cc[VINTR] = 0;
	t.c_cc[VSUSP] = 0;
#ifdef VDSUSP
  t.c_cc[VDSUSP] = 0;
#endif
#ifdef VLNEXT
  t.c_cc[VLNEXT] = 0;
#endif
	tcsetattr (fileno(stdin), TCSANOW, &t);

/*	raw();*/
	nonl();
	noecho();
	keypad (stdscr, TRUE);
#else
	ioctl(0, TCGETA, &ostate);
	nstate = ostate;
	nstate.c_lflag &= ~(ICANON|ECHO|ISIG);
	nstate.c_cc[4] = 1;
	ioctl(0, TCSETAW, &nstate);
#endif
#endif
#endif
return 0;
}

/*
 * This function gets called just
 * before we go back home to the command interpreter.
 * On VMS it puts the terminal back in a reasonable state.
 * Another no-operation on CPM.
 */
int ttclose()
{
#if	VMS
	int	status;
	int	iosb[1];

	ttflush();
	status = SYS$QIOW(EFN, iochan, IO$_SETMODE, iosb, 0, 0,
		 oldmode, sizeof(oldmode), 0, 0, 0, 0);
	if (status!=SS$_NORMAL || (iosb[0]&0xFFFF)!=SS$_NORMAL)
		exit(status);
	status = SYS$DASSGN(iochan);
	if (status != SS$_NORMAL)
		exit(status);
#endif
#if	CPM
#endif
#if	AtST
	if (vidrev) {
		/* set background color to white, foreground to black */
		ttputs ("\033c0\033bo");
	}
	vt52nrml();
#endif
#if ST_DA
	ttputs ("\033f");		/* hide text cursor */
#endif
#if	W32
printf("\x1b[0m");	
SetConsoleMode(stdoutHandle, outModeInit);
#endif
#if	MSDOS
	if (vidmode >= 0)
		textmode (origvidmode);
	normvideo();
/*	putch(' ');		/ * put norm attr into effect, not neccesary */
	putch('\r'); putch('\n'); /* new line FreeDOS needed it */
/*	clreol();		/ * not neccesary */
#endif
#if	V7
#if     !SYS_V
	ttflush();
#if SLEEP
	sleep(SLEEP);			/* mb: still seems necessary!	*/
#endif
	stty(1, &ostate);
#else
#if	CURSES
	endwin();
#else
#if	VT100
#endif
#if	HP700
	ttputs("\033&s0A");
#endif
	ioctl(0, TCSETAW, &ostate);
#endif
#endif
	putchar('\n');
	fflush(stdout);
#endif
return 0;
}

/*
 * Write a character to the display.
 * On VMS, terminal output is buffered, and
 * we just put the characters in the big array,
 * after cheching for overflow. On CPM terminal I/O
 * unbuffered, so we just write the byte out.
 * Ditto on MS-DOS (use the very very raw console
 * output routine).
 */
int ttputc(c)
	int c;
{
#ifdef DEBUG
	fprintf (stderr, "putc:%c\n",c);
#endif
#if	VMS
	if (nobuf >= NOBUF)
		ttflush();
	obuf[nobuf++] = c;
#endif
#if	CPM
	bios(BCONOUT, c, 0);
#endif
#if	AtST
	Bconout(2, c);
#endif
#if	MSDOS
	putch(c);		/* Turbo C library MS-DOS call	*/
#endif
#if	W32
	fputc(c, stdout);
#endif
#if	V7
#if	CURSES
	addch(c);
#else
	fputc(c, stdout);
#endif
#endif
return 0;
}

void ttputs(str)			/* mb: added */
	register char *str;
{
	register int c;
	while ((c=(*str++))!='\0')
		ttputc(c);
}

/*
 * Flush terminal buffer. Does real work
 * where the terminal output is buffered up. A
 * no-operation on systems where byte at a time
 * terminal I/O is done.
 */
int ttflush()
{
#if	VMS
	int	status;
	int	iosb[2];

	status = SS$_NORMAL;
	if (nobuf != 0) {
		status = SYS$QIOW(EFN, iochan, IO$_WRITELBLK|IO$M_NOFORMAT,
			 iosb, 0, 0, obuf, nobuf, 0, 0, 0, 0);
		if (status == SS$_NORMAL)
			status = iosb[0] & 0xFFFF;
		nobuf = 0;
	}
	return (status);
#endif
#if	CPM
#endif
#if	MSDOS
#endif
#if	AtST
#endif
#if	V7
#if	CURSES
	refresh();
#else
	fflush(stdout);
#endif
#endif
return 0;
}

/*
 *  mb: find out if there is a char waiting to be read.
 */
int
ttpending()
{
#if V7
#ifdef FIONREAD
	long c;
  char *cp;
  cp = (char *) &c;
	if (ioctl(0, FIONREAD, (struct sgttyb *) cp) == -1)
		return (FALSE);
	return (c > 0);
#else
	return (FALSE);
#endif
#endif
#if AtST
	return (Cconis());
#endif
#if MSDOS
	return (bioskey(1));		/* Turbo C specific */
#endif
#if (CPM | VMS | W32)
	return (FALSE);
#endif
}

/*
 * Read a character from the terminal,
 * performing no editing and doing no echo at all.
 * More complex in VMS than almost anyplace else, which
 * figures. Very simple on CPM, because the system can
 * do exactly what you want.
 */
int ttgetc()
{
#if	VMS
	int	status;
	int	iosb[2];
	int	term[2];

	while (ibufi >= nibuf) {
		ibufi = 0;
		term[0] = 0;
		term[1] = 0;
		status = SYS$QIOW(EFN, iochan, IO$_READLBLK|IO$M_TIMED,
			 iosb, 0, 0, ibuf, NIBUF, 0, term, 0, 0);
		if (status != SS$_NORMAL)
			exit(status);
		status = iosb[0] & 0xFFFF;
		if (status!=SS$_NORMAL && status!=SS$_TIMEOUT)
			exit(status);
		nibuf = (iosb[0]>>16) + (iosb[1]>>16);
	if (nibuf == 0) {
			status = sys$qiow(EFN, iochan, IO$_READLBLK,
				 iosb, 0, 0, ibuf, 1, 0, term, 0, 0);
			if (status != SS$_NORMAL
			|| (status = (iosb[0]&0xFFFF)) != SS$_NORMAL)
				exit(status);
			nibuf = (iosb[0]>>16) + (iosb[1]>>16);
		}
	}
	return (ibuf[ibufi++] & 0xFF);		/* Allow multinational	*/
#endif
#if	CPM
	return (biosb(BCONIN, 0, 0));
#endif
#if	AtST
	register long key;
	register int  kbs;
	register int  c;
	register int  k;

	key = Bconin(2);	/* key code in upper word */
	kbs = Kbshift(-1);	/* read shift and alt keys */
	c = (int)(key & 0xFF);
	k = (int)((key >> 16) & 0xFF);
	if ( k==0x0E || k==0x53		/* bs & del	*/
	  || k==0x4B || k==0x4D		/* arrow keys	*/
	  || k==0x48 || k==0x50
	  || k==0x52 || k==0x47		/* ins & clr	*/
	  || (c==0 && k!=0x03) ) {	/* other function keys or
					   Alt-, but leave ^@	*/
		if (k>=0x54 && k<=0x5D)	/* shifted F1..f10 ->	*/
			k -= 0x19;	/*    unshifted codes	*/
		c = FUNC | k;
		if (kbs & 0x0F)		/* Ctrl, Shift or Alt?	*/
			c |= SHFT;
	}
	return (c);
#endif
#if	(MSDOS | W32)
	int k;

	k = getch();
	if (k)				/* not a function key	*/
		return (k);
	k = getch();			/* get scan code	*/
	if (k >= 0x5E && k <= 0x67)	/* CNTL-Fn		*/
		k -= 0x0A;
	if (k >= 0x68 && k <= 0x71)	/* ALT-Fn		*/
		k -= 0x14;
	return (FUNC | k);
#endif
#if	V7
#if	CURSES
	int ch;

	ch = getch();
#ifdef hpux
	/* hp-emulation returns a "RETURN" after every function-key! */
	if (hpterm && ch>=KEY_F(0) && ch<=KEY_F(12))
		getch();
#endif
	return ch;
#else
	return(fgetc(stdin));
#endif
#endif
}

/*
 *  mb: output to a hardcopy device (port identified by 'hdev').
 */
int
hardputc(c)
	int c;
{
#if AtST | MSDOS
	int s;
	extern int hdev;
#endif
#if AtST
	return (Bconout (hdev, c));
#endif
#if MSDOS
	if (hdev == PARPORT) {
		s = biosprint (0,c,0);	/* Turbo C.  LPT1. */
		if (s & 0x0001)
			return (FALSE);
		return (TRUE);
	} else {
		s = bioscom (1,c,0);	/* Turbo C.  COM1. */
		if (s & 0x8000)
			return (FALSE);
		return (TRUE);
	}
#endif
#if (V7 | VMS | CPM | W32)
	return (FALSE);
#endif
}

/*
 * file vt52.c:
 *
 * The routines in this file
 * provide support for VT52 style terminals
 * over a serial line. The serial I/O services are
 * provided by routines in "termio.c". It compiles
 * into nothing if not a VT52 style device. The
 * bell on the VT52 is terrible, so the "beep"
 * routine is conditionalized on defining BEL.
 */

#if	VT52

#define	BIAS	0x20			/* Origin 0 coordinate bias.	*/
#define	ESC	0x1B			/* ESC character.		*/
#define BEL	0x07			/* ascii bell character		*/

extern	int	vt52move();		/* Forward references.		*/
extern	int	vt52eel();
extern	int	vt52eep();
extern  int	vt52hglt();
extern  int	vt52nrml();
extern	int	vt52beep();
extern	int	vt52open();

/*
 * Dispatch table. All the
 * hard fields just point into the
 * terminal I/O code.
 */
TERM	term	= {
	NROW-1,
	NCOL,
	vt52open,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	vt52move,
	vt52eel,
	vt52eep,
	vt52beep,
	vt52hglt,
	vt52nrml,
	ttpending
};

vt52move(row, col)
{
	ttputs("\033Y");
	ttputc(row+BIAS);
	ttputc(col+BIAS);
}

vt52eel()
{
	ttputs("\033K");
}

vt52eep()
{
	ttputs("\033J");
}

vt52beep()
{
	ttputc(BEL);
	ttflush();
}

vt52hglt()
{
	ttputs("\033p");
}

vt52nrml()
{
	ttputs("\033q");
}

vt52open()
{
#if	V7
	register char *cp;
	char *getenv();

	if ((cp = getenv("TERM")) == NULL) {
		puts("Shell variable TERM not defined!");
		exit(1);
	}
	if (strcmp(cp, "vt52") != 0 && strcmp(cp, "z19") != 0) {
		puts("Terminal type not 'vt52'or 'z19' !");
		exit(1);
	}
#endif
	ttopen();
}

#endif

/*
 * file ansi.c:
 *
 * The routines in this file
 * provide support for ANSI style terminals
 * over a serial line. The serial I/O services are
 * provided by routines in "termio.c". It compiles
 * into nothing if not an ANSI device.
 */

#if	ANSI

#define BEL	0x07			/* BEL character.		*/
#define ESC	0x1B			/* ESC character.		*/

extern  int	ttopen();		/* Forward references.		*/
extern  int	ttgetc();
extern  int	ttputc();
extern  int	ttflush();
extern  int	ttclose();
extern  int	ansimove();
extern  int	ansieeol();
extern  int	ansieeop();
extern  int	ansihglt();
extern  int	ansinrml();
extern  int	ansibeep();
extern  int	ansiopen();

/*
 * Standard terminal interface
 * dispatch table. Most of the fields
 * point into "termio" code.
 */
TERM	term	= {
	NROW-1,
	NCOL,
	ansiopen,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	ansimove,
	ansieeol,
	ansieeop,
	ansibeep,
	ansihglt,
	ansinrml,
	ttpending
};

ansiparm(n)
register int	n;
{
	register int	q;

	q = n/10;
	if (q != 0)
		ansiparm(q);
	ttputc((n%10) + '0');
}

ansimove(row, col)
{
	ttputc(ESC);
	ttputc('[');
	ansiparm(row+1);
	ttputc(';');
	ansiparm(col+1);
	ttputc('H');
}

ansieeol()
{
	ttputs("\033[K");
}

ansieeop()
{
	ttputs("\033[J");
}

ansibeep()
{
	ttputc(BEL);
	ttflush();
}

ansihglt()
{
	ttputs("\033[7m");
}

ansinrml()
{
	ttputs("\033[m");
}

ansiopen()
{
#if	V7
	register char *cp;
	char *getenv();

	if ((cp = getenv("TERM")) == NULL) {
		puts("Shell variable TERM not defined!");
		exit(1);
	}
	if (strncmp(cp, "vt", 2) != 0 &&
	    strncmp(cp, "ansi", 4) != 0 &&
	    strncmp(cp, "putty", 5) != 0 &&
	    strncmp(cp, "linux", 5) != 0 &&
	    strncmp(cp, "console", 7) != 0 &&
	    strncmp(cp, "xterm", 5) != 0) {
		puts("Terminal type not 'ansi'!");
		sleep(2);
/*		exit(1); */
	}
#endif
	ttopen();
}

#endif

/*
 * file hp700.c:
 *
 * The routines in this file
 * provide support for HP style terminals
 * over a serial line. The serial I/O services are
 * provided by routines in "termio.c". It compiles
 * into nothing if not an HP device.
 */

#if	HP700

#define BEL	0x07			/* BEL character.		*/
#define ESC	0x1B			/* ESC character.		*/

extern  int	ttopen();		/* Forward references.		*/
extern  int	ttgetc();
extern  int	ttputc();
extern  int	ttflush();
extern  int	ttclose();
extern  int	hpmove();
extern  int	hpeeol();
extern  int	hpeeop();
extern  int	hphglt();
extern  int	hpnrml();
extern  int	hpbeep();
extern  int	hpopen();

/*
 * Standard terminal interface
 * dispatch table. Most of the fields
 * point into "termio" code.
 */
TERM	term	= {
	NROW-1,
	NCOL,
	hpopen,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	hpmove,
	hpeeol,
	hpeeop,
	hpbeep,
	hphglt,
	hpnrml,
	ttpending
};

hpmove(row, col)
{
/* cup=\E&a%p1%dy%p2%dC
   cup=\E[%i%p1%d;%p2%dH
 */
	ttputc(ESC);
	ttputc('&');
	ttputc('a');
	hpparm(row);
	ttputc('y');
	hpparm(col);
	ttputc('C');
}

hpeeol()
{
/* el=\EK
   el=\E[K$<3>
 */
	ttputs("\033K");
}

hpeeop()
{
	ttputs("\033J");
}

hpbeep()
{
	ttputc(BEL);
	ttflush();
}

hphglt()
{
	ttputs("\033&dB");
}

hpnrml()
{
	ttputs("\033&d@");
}

hpparm(n)
register int	n;
{
	register int	q;

	q = n/10;
	if (q != 0)
		hpparm(q);
	ttputc((n%10) + '0');
}

hpopen()
{
#if	V7
	register char *cp;
	char *getenv();

	if ((cp = getenv("TERM")) == NULL) {
		puts("Shell variable TERM not defined!");
		exit(1);
	}
	if (strncmp(cp, "hp", 2) != 0 &&
	    strncmp(cp, "700", 3) != 0) {
		puts("Terminal type not 'hp'!");
		sleep(2);
/*		exit(1); */
	}
#endif
	ttopen();
	ttputs("\033&s1A");
}

#endif

/*
 * "file" tcconio.c:
 * These routines provide support for Turbo-C direct console output.
 */

#if	TCCONIO

#define BEL 0x07

extern  int	ttopen();		/* Forward references.		*/
extern  int	ttgetc();
extern  int	ttputc();
extern  int	ttflush();
extern  int	ttclose();
extern  void	coniomove();
extern  void	conioeeop();
extern  void	coniohglt();
extern  int	conionrml();
extern  void	coniobeep();
extern	void	clreol();

/*
 * Standard terminal interface
 * dispatch table. Most of the fields
 * point into "termio" code.
 */
TERM	term	= {
	NROW-1,
	NCOL,
	ttopen,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	coniomove,
	clreol,
	conioeeop,
	coniobeep,
	ttreverse,
	ttreverse,
	ttpending
};

void coniomove(row, col)
{
	gotoxy (col+1, row+1);
}

void conioeeop()
{
	int i;

	clreol();
	for (i=wherey()+1; i<=term.t_nrow; i++) {
		gotoxy (1, i);
		clreol();
	}
	gotoxy (1, 1);
}

void coniobeep()
{
	ttputc(BEL);
}

#endif

/*
 * file: tcap.c
 *
 * This file does the term stuff for systems
 * that have 'termcap' flexibility.
 */

#if TERMCAP

#define BEL	0x07
#define ESC	0x1B

extern int	ttopen();
extern int	ttgetc();
extern int	ttputc();
extern int	ttflush();
extern int 	ttclose();
extern int 	tcapmove();
extern int 	tcapeeol();
extern int	tcapeeop();
extern int	tcaphglt();
extern int	tcapnrml();
extern int	tcapbeep();
extern int	tcapopen();
extern int	tput();
extern char	*tgoto();

/* mb: following 3 entries added: */

extern int	tgetent();
extern char	*tgetstr();
extern int	tputs();

#define TCAPSLEN 315

char tcapbuf[TCAPSLEN];
char	*CM=NULL,
	*CL=NULL,
	*CE=NULL,
	*CD=NULL,
	*MR=NULL,
	*ME=NULL;

TERM term = {
	0,		/* will be set from termcap entry */
	0,
	tcapopen,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	tcapmove,
	tcapeeol,
	tcapeeop,
	tcapbeep,
	tcaphglt,
	tcapnrml,
	ttpending
};

tcapopen()
{
	char *getenv();
	char *t, *p, *tgetstr();
	char tcbuf[1024];
	char *tv_stype;
	char err_str[72];
	int	n;

	if ((tv_stype = getenv("TERM")) == NULL)
	{
		puts("Environment variable TERM not defined!");
		exit(1);
	}

	if((tgetent(tcbuf, tv_stype)) != 1)
	{
		sprintf(err_str, "Unknown terminal type %s!", tv_stype);
		puts(err_str);
		exit(1);
	}

	/* mb: if these have not been set from the command
	 *     line, get the screen size from the termcap file.
	 */
	if (! term.t_ncol)
		term.t_ncol = ((n=tgetnum("co"))>0? n : NCOL);
	if (! term.t_nrow)
		term.t_nrow = ((n=tgetnum("li"))>0? n-1 : NROW-1);

	p = tcapbuf;

	CD = tgetstr("cd", &p);
	CE = tgetstr("ce", &p);
	CM = tgetstr("cm", &p);
	ME = tgetstr("me", &p);
	MR = tgetstr("mr", &p);
	if (ME == NULL)
		ME = tgetstr("se", &p);
	if (MR == NULL)
		MR = tgetstr("so", &p);
	if (ME == NULL)
		MR = NULL;

	if(CD==NULL || CM==NULL || CE==NULL) {
		puts("Termcap entry insufficient!\n");
		exit(1);
	}

	if (p >= &tcapbuf[TCAPSLEN])
	{
		puts("Terminal description too big!\n");
		exit(1);
	}
	ttopen();
}

tcapmove(row, col)
register int row, col;
{
	putpad(tgoto(CM, col, row));
}

tcapeeol()
{
	putpad(CE);
}

tcapeeop()
{
	putpad(CD);
}

tcapbeep()
{
	ttputc(BEL);
}

tcaphglt()
{
	if (MR != NULL)
		putpad(MR);
}

tcapnrml()
{
	if (ME != NULL)
		putpad(ME);
}

putpad(str)
char	*str;
{
	tputs(str, 1, ttputc);
}
#endif

/*
 * file: tcurs.c
 *
 * This file does the term stuff for systems
 * that have 'curses' flexibility.
 */

#if CURSES

#define ESC	0x1B

int tcuropen()
{
	ttopen();
	return 0;
}

int tcurmove(row, col)
register int row, col;
{
	move(row, col);
	return 0;
}

int tcureeol()
{
	clrtoeol();
	return 0;
}

int tcureeop()
{
	clear();
	return 0;
}

int tcurbeep()
{
	beep();
	return 0;
}

int tcurhglt()
{
	standout();
	return 0;
}

int tcurnrml()
{
	standend();
	return 0;
}

void putpad(str)
char	*str;
{
	tputs(str, 1, ttputc);
}

/*
extern int	tput();
extern char	*tgoto();
 */

TERM term = {
	0,		/* will be set from curses entry */
	0,
	tcuropen,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	tcurmove,
	tcureeol,
	tcureeop,
	tcurbeep,
	tcurhglt,
	tcurnrml,
	ttpending
};
#endif
