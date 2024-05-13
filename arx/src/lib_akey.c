/* lib_akey.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arx_def.h>

#define KEYLENGTH	4

static char *rcsinfolib = "$""Id: arxlib " ARXREL "." ARXVER " " PLATFORM CCOMPILER " $";

int lib_akey (char *logo, int show)
{
return RET_SUCCESS;
}

char *lib_veid (void)
{
return rcsinfolib;
}
