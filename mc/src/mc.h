/* mc.h,v 1.7 1997/05/21 15:26:58 axel */

#include "mcver.h"

#define makemsg(n,s)	n,
enum	{
#include "mcmsg.h"
	MSGEND2
	};
#undef makemsg
#define MAXMSGWIDTH	80

#include "mctext.h"
#include "mcmnud.h"
#include "mcbool.h"
#include "mcelldef.h"
#include "mckey.h"

#ifndef COMMON
#include "mcommon.h"
#endif
