/*
**	lib_getch.c
**
**	The routine getch().
**
** $Log: cur_getc.c,v $
** Revision 1.2  1996/10/15 19:22:33  axel
** fix co bug
**
 * Revision 1.1  1995/05/21  08:23:43  axel
 * Initial revision
 *
 * Revision 2.2  91/02/09  13:37:40  munk   
 * Conditional code added for systems that don't support ioctl with
 * the command FIONREAD (use fcntl O_NDELAY).
 * Bug fixed in kgetch(), do not put EOF in buffer.
 * Save the old alarm-timer and restart it, after use by kgetch().
 *
 * Revision 2.1  82/10/25  14:47:29  pavel
 * Added Copyright Notice
 * 
 * Revision 2.0  82/10/25  13:45:19  pavel
 * Beta-one Test Release
 * 
**
*/

#include <signal.h>
#ifdef COHERENT
#include <sys/fcntl.h>
#else
#include <fcntl.h>
#endif
#include <curses.h>
#include "cur_priv.h"

#define nextc()       (SP->_backcnt > 0  ?  SP->_backbuf[--SP->_backcnt] \
                                         :  getc(SP->_ifp))
				       
#define putback(ch)   SP->_backbuf[SP->_backcnt++] = ch


cur_getc(win)
register WINDOW	*win;
{
	bool		setHere = FALSE; /* cbreak mode was set here         */
	register short	ch;		 /* 'short' because of keypad codes  */
        short   kgetch();

#ifdef TRACE
	if (_tracing)
	    _tracef("wgetch(%o) called", win);
#endif

	if (! win->_scroll  &&  (win->_flags & _FULLWIN)
						&&  win->_curx == win->_maxx
						&&  win->_cury == win->_maxy)
	    return(ERR);

#ifdef FIONREAD
	if (win->_nodelay)
	{
	    long count;

	    ioctl(fileno(SP->_ifp), FIONREAD, &count);

	    if (! count)
		return(-1);
	}
#else
	if (win->_nodelay)
	{
		int flags;

		flags = fcntl(fileno(SP->_ifp), F_GETFL, 0);
		fcntl(fileno(SP->_ifp), F_SETFL, flags | O_NDELAY);
		ch = fgetc(SP->_ifp);
		fcntl(fileno(SP->_ifp), F_SETFL, flags);
		if (ch == -1)
			return(-1);
		ungetc(ch, SP->_ifp);
	}
#endif

	if (SP->_echo  &&  ! (SP->_raw  ||  SP->_cbreak))
	{
		cbreak();
		setHere = TRUE;
	}

        if (win->_use_keypad)
            ch = kgetch();
        else
	    ch = nextc();

	if (SP->_echo  &&  ch < 0400 && ch >= 0)/* ch < 0400 => not a keypad key */
	{
	    mvwaddch(curscr, win->_begy + win->_cury,
                             win->_begx + win->_curx, ch | win->_attrs);
	    waddch(win, ch | win->_attrs);
	}

	if (setHere)
	    nocbreak();

	return(ch);
}

/*
**      short
**      kgetch()
**
**      Get an input character, but take care of keypad sequences, returning
**      an appropriate code when one matches the input.  After each character
**      is received, set a one-second alarm call.  If no more of the sequence
**      is received by the time the alarm goes off, pass through the sequence
**      gotten so far.
**
*/

static  bool    alarmed;

static
short
kgetch()
{
        struct try	*ptr;
	register char	ch;
	char		buffer[10]; /* Assume no sequences longer than 10 */
	register char	*bufp = buffer;
	int		(*oldsig)();
	int		oldalarm;
	int		sigalrm();

	ptr = SP->_keytry;
	
	oldalarm = alarm(0);
	oldsig = signal(SIGALRM, sigalrm);
	signal(SIGALRM, oldsig);
	alarm(oldalarm);
	alarmed = FALSE;
	
	do
	{
	    ch = nextc();
	    if (ch != EOF)		/* do not put EOF in buffer */
	        *(bufp++) = ch;

	    if (alarmed)
	        break;
	    
	    while (ptr != NULL  &&  ptr->ch != ch)
	        ptr = ptr->sibling;
	    
	    if (ptr != NULL)
	    {
	        if (ptr->value != 0)
		{
		    signal(SIGALRM, oldsig);
		    alarm(oldalarm);
		    return(ptr->value);
		}
		else
		{
		    ptr = ptr->child;
		    signal(SIGALRM, sigalrm);
		    alarm(1);
	        }
	    }
	    
	} while (ptr != NULL);
	
	signal(SIGALRM, oldsig);
	alarm(oldalarm);
	
	while (--bufp > buffer)
	    putback(*bufp);
	    
	return(*bufp);
}


static
sigalrm()
{
        alarmed = TRUE;
	signal(SIGALRM, sigalrm);
}
