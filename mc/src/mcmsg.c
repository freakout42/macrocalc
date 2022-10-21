/* $Id: mcmsg.c,v 1.4 1995/05/05 05:06:40 axel Rel $
 * MicroCALC Spreadsheet message get: faster form of
 * awk -F"\t	" '$1 == '$1' {print $2}' <$ARX/lib/mcmsg.rdb 1>&2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
char *message = NULL;
char line[256];

if (argc != 2)
	{
	fprintf (stderr, "usage: mcmsg messagenumber <messagefile\n");
	exit(2);
	}
while (fgets(line, sizeof(line), stdin) != NULL)
	{
	if (strcmp (argv[1], strtok (line, "\t"))) continue;
	if ((message = strtok (NULL, "\t")) == NULL)
		putchar ('\n');
	else
		fputs(message, stdout);
	exit(0);
	}
return 1;
}
