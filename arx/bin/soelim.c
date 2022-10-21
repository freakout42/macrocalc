/* $Id: soelim.c,v 1.3 1999/07/17 18:26:18 axel Exp $
 */
#include <stdio.h>
#include <string.h>

fatal (msg)
	char *msg;
{
fprintf (stderr, "soelim: %s\n", msg);
exit (1);
}

readfile (f)
	FILE * f;
{
FILE	*incf;
char	line[256];

while (fgets (line, sizeof(line), f) != NULL)
	{
	if (!strncmp (line, ".so ", 4))
		{
		line[strlen(line)-1] = '\0';
		if ((incf = fopen (line+4, "r")) == NULL)
			fatal ("cannot open");
		readfile (incf);
		}
	else	printf ("%s", line);
	}
}

int main()
{
readfile (stdin);
exit (0);
}
