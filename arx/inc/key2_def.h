/* key2_def.h */

/* keys not defined by curses.h */
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

/* user colors */
/* enum { COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE };
          =0           =1         =2           =3            =4          =5             =6          =7             */

enum { /* pair index   */
  COL_DEFAULT,         /* = 0 (-1,-1) */
  COL_BLACK,           /* = 1 */
  COL_RED,             /* = 2 */
  COL_GREEN,           /* = 3 */
  COL_YELLOW,          /* = 4 */
  COL_BLUE,            /* = 5 */
  COL_MAGENTA,         /* = 6 */
  COL_CYAN,            /* = 7 */
  COL_WHITE,           /* = 8 */
  TEXTCOLOR,           /* = 9 */
  EOFCOLOR,            /* =10 */
  ERRORCOLOR,          /* =11 */
  VALUECOLOR,          /* =12 */
  FORMULACOLOR,        /* =13 */
  STRINGCOLOR,         /* =14 */
  BLANKCOLOR,          /* =15 */
  COMMANDCOLOR,        /* =16 */
  HIGHLIGHTCOLOR,      /* =17 */
  HIGHLIGHTERRORCOLOR, /* =18 */
  HEADERCOLOR,         /* =19 */
  CURHEADERCOLOR,      /* =20 */
  MARKCOLOR,           /* =21 */
  AUTOCALCCOLOR,       /* =22 */
  FORMDISPLAYCOLOR,    /* =23 */
  MESSAGECOLOR,        /* =24 */
  PROMPTCOLOR,         /* =25 */
  INPUTCOLOR,          /* =26 */
  CELLCONTENTSCOLOR,   /* =27 */
  COL_UNDEF
};

/* 16 colors - not used
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 128
 */
