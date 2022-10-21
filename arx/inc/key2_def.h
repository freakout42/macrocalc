#define MAXSCREENWIDTH	256
#define MAXINPUT	32767
#define	KEY_BS		'\b'
#define	KEY_RETURN	'\n'
#define	KEY_RRETURN	'\r'
#define	KEY_TAB		'\t'
#define	KEY_ESC		'\033'
#define	KEY_CTRL(c)	(c+1-'A')
#ifndef KEY_END
#define KEY_END KEY_LL
#endif
#ifndef KEY_BTAB
#define KEY_BTAB 0534
#endif
