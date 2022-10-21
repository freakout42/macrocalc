/* $Id: mcftab.h,v 1.2 1997/04/04 06:38:34 axel Exp $
 */

extern struct Functab
	{
	char		*name;
	int		token;
	unsigned char	opcode;
	double		(*func)();
	} functab[];
