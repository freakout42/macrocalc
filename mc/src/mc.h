/* mc.h */

#include "mcver.h"

#ifdef WIN32
#define MSGINLINE
#endif

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
