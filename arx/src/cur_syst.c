/* $Id: cur_syst.c,v 1.10 2012/02/20 20:00:35 axel Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#if defined(__hpux) || defined(COHERENT) || defined(__FreeBSD__) || defined(linux) || defined(_AIX) || defined(__DARWIN_UNIX03)
#include <sys/wait.h>
#else
#include <wait.h>
#endif
#include "arx_def.h"
#include "cur_def.h"

int cur_syst (WINDOW *w, char *cmd, int prompt)
{
int	pid, wt, status;
#ifdef SYSTEM
char	command[1024];
#endif

cur_rest();

#ifdef SYSTEM
if (cur_redir >= 3)	sprintf (command, "%s 2>&%d", cmd, fileno(std_err));
else			strcpy (command, cmd);
status	= system (command);

#else
if ((pid = fork()) == 0)
	{
	if (cur_redir >= 3)
		{
		close (STDERR_FILENO);
		if (dup (fileno(std_err)) != STDERR_FILENO) exit(126);
		}
	for (wt = STDERR_FILENO+1; wt < _NFILE; wt++) close (wt);
	signal (SIGINT, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
	signal (SIGPIPE, SIG_DFL);
	execl ("/bin/sh", "sh", "-c", cmd, NULL);
	exit (127);
	}
while ((wt = wait(&status)) != pid && wt != -1 && errno != EINTR)
#ifdef	DEBUG
	fprintf (stderr, "status=%d errno=%d\n", wt, errno)
#endif
	;
if (wt == -1) status = -1;
#endif

/*lib_flus (stdin);*/
cur_goll();
cur_fixt();
if (prompt)
	{
	printf ("[Press any key to continue]");
	cur_getp (w);
	}
return status;
}
