/* lib_twpo.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arx_def.h>
#include <fcntl.h>

#define MAXPARM 256

int lib_twpo (FILE **in, FILE **out, char *cmd)
{
int	inpipe[2];
int	outpipe[2];
int	i, abspath=0;
char	*parms[MAXPARM];
char	ws[3] = " \t";
char	mycmd[MAXPATH];

#ifdef DEBUG
fprintf (stderr, "lib_twpo: start\n");
#endif

strcpy(mycmd, cmd);
parms[0] = strtok (mycmd, ws);
if (strchr(cmd,'/')) { abspath = 1; }
else {
if (lib_path (getenv("PATH"), parms[0], X_OK) == NULL) return RET_FATAL;
}
#ifdef DEBUG
fprintf (stderr, "lib_twpo: path done\n");
#endif
for (i=1; i<MAXPARM; i++) if ((parms[i] = strtok (NULL, ws)) == NULL) break;
if (pipe (inpipe) || pipe (outpipe)) return RET_FATAL;
#ifdef DEBUG
fprintf (stderr, "lib_twpo: pipe done\n");
#endif
switch (fork())
 {
 case FAIL:	return RET_ERROR;
 case CHILD:
	{
	if (close(STDIN_FILENO) || close(STDOUT_FILENO) || close(STDERR_FILENO))
		exit (EXIT_FATAL);
	if (	dup (outpipe[READ]) != STDIN_FILENO	||
		dup (inpipe[WRITE]) != STDOUT_FILENO	||
		dup (inpipe[WRITE]) != STDERR_FILENO)	exit (EXIT_FAILURE);
	if (	close (outpipe[READ])	||
		close (outpipe[WRITE])	||
		close (inpipe[READ])	||
		close (inpipe[WRITE])	)	exit (EXIT_FAILURE);
	if (abspath) execv (mycmd, parms); else execvp (mycmd, parms);
	fprintf (stderr, "execv \"%s\" failed\n", cmd);
	exit (EXIT_FATAL);
	}
 default:
	{
	if (	close (outpipe[READ])	||
		close (inpipe[WRITE]))	return RET_ERROR;
	if (*in==NULL) fcntl (inpipe[READ], F_SETFL, O_NDELAY);
	fcntl (inpipe[READ], F_SETFD, 1);
	fcntl (outpipe[WRITE], F_SETFD, 1);
	*in	= fdopen (inpipe[READ], "r");
	*out	= fdopen (outpipe[WRITE], "w");
	setbuf (*out, NULL);
	if (*in == NULL || *out == NULL) return RET_ERROR;
	}
 }
return RET_SUCCESS;
}
