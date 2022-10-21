/*	str_abbr.c
 *	STRing ABBRiviation
 *	test if *a is a valid abbriviation of *s
 *
 *	V01-001	26-MAR-90	Reinhold	Original
 *	V01-002	25-JUL-90	Reinhold	null string not valid
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <arx_def.h>

int	str_abbr (s, a)
	const char *s; const char *a;

{
if (!*a) return FALSE;
while (toupper (*s) == toupper (*a) && isalnum (*s)) s++, a++;
return *a == '\0';
}
