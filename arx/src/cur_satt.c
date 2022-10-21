/* $Id: cur_satt.c,v 1.4 1995/02/27 18:25:54 axel Rel $
 */

#include <cur_def.h>
#include <term.h>

int cur_satt (WINDOW *w, unsigned int attrib)
{
if (attrib & A_REVERSE)
	{
	if (enter_reverse_mode==NULL)	/* terminal has no reverse mode	*/
		attrib |= A_STANDOUT;	/* use standout instead		*/
	}
if (attrib & A_BLINK)
	{
	if (enter_blink_mode==NULL)	/* terminal has no blink mode	*/
		attrib |= A_STANDOUT;	/* use standout instead		*/
	}
return wattrset (w, attrib);
}
