/* mcinter.c
 * #define DEBUG
 */

#include <string.h>
#include <stdlib.h>
#ifdef DEBUG_X
#include <fcntl.h>
#endif
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcgetmsg.h"
#include "mcmessag.h"
#include "mcfile.h"
#include "mcrun.h"
#include "mcgetmnu.h"
#include "mcrmacro.h"
#include "mcdisply.h"
#include "mcolor.h"
#include "mcinter.h"

void mcinter (void)
{
char	m[MAXSCREENWIDTH+1]	= "";

#ifdef DEBUG
int	flags, fd;
#endif

if (getmessage (m, MSGCOMMAND))
	{
	fprintf (stderr, "mc: no messages.\n");
	exit (EXIT_FAILURE);
	}
msgcommand = strdup (m);
if (msgcommand == NULL)
	{
	fprintf (stderr, "mc: no memory.\n");
	exit (EXIT_FATAL);
	}
if (readmenus())
	{
	getmessage (m, MSGMENUFILE);
	fprintf (stderr, "mc: %s.\n", m);
	exit (EXIT_FAILURE);
	}

#ifdef DEBUG
	#define	CURINIT	0
#else
	#define	CURINIT	3
#endif
if (cur_init(CURINIT) == NULL)
	{
	fprintf (stderr, "mc: no curses.\n");
	exit (EXIT_FAILURE);
	}
if (colors && has_colors()) {
  start_color();
  mcolor0();
} else {
  colors = FALSE;
}
#ifdef DEBUG
/* fd = fileno(std_erread);
 * flags = fcntl (fd, F_GETFL, 0);
 * fcntl (fd, F_SETFL, flags & ~O_NDELAY);
 * only cornell curses:
 * traceon();
 */
#endif

clearsheet();
if (*filename)	loadsheet (NOPROMPT);
if (*macroname || *macroexec) runmacro (NOPROMPT);
/*else errorstr (logo);
if (license!=0) errormsg(MSGDEMO);*/
redrawscreen();
run();
cur_exit (EOF);
}
