#include <stdio.h>
#include "lotus.h"
main()
{
#define check(st) printf ("%16.16s: sizeof:%4d val:%4d\n", \
				#st, sizeof(r.data.st), LL_##st);

Lotrec r;

check(bof);
/*
 * check(eof);
 * check(calcmode);
 * check(calcorder);
 */
check(split);
check(sync);
check(window1);
check(colw1);
check(name);
check(integer);
check(number);
check(label);
check(formula);
check(protec);
}
