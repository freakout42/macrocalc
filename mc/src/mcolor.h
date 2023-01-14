/* mcolor.h,v 1.2 1995/02/26 17:04:33 axel */

#define TEXTCOLOR A_NORMAL
#define EOFCOLOR A_BOLD
#define ERRORCOLOR A_BLINK
#define VALUECOLOR A_NORMAL
#define FORMULACOLOR A_NORMAL
#define STRINGCOLOR A_NORMAL
#define BLANKCOLOR A_NORMAL
#define HEADERCOLOR A_REVERSE
#define CURHEADERCOLOR A_BOLD
#define HIGHLIGHTCOLOR A_REVERSE
#define HIGHLIGHTERRORCOLOR (A_REVERSE|A_BLINK)
#define MARKCOLOR A_REVERSE
#define AUTOCALCCOLOR A_REVERSE
#define FORMDISPLAYCOLOR A_REVERSE
#define MESSAGECOLOR (A_BOLD|A_BLINK)
#define PROMPTCOLOR A_BOLD
#define INPUTCOLOR A_REVERSE
#define COMMANDCOLOR A_UNDERLINE
#define CELLCONTENTSCOLOR A_NORMAL
#define	BOLDCOLOR A_BOLD
#define	ITALICOLOR A_UNDERLINE

enum { /* pair index   */
  COL_BLACK,     /* =0 */
  COL_RED,       /* =1 */
  COL_GREEN,     /* =2 */
  COL_YELLOW,    /* =3 */
  COL_BLUE,      /* =4 */
  COL_MAGENTA,   /* =5 */
  COL_CYAN,      /* =6 */
  COL_WHITE,     /* =7 */
	COL_HEADER,    /* =8 */
  COL_UNDEF
};

void mcolor0(void);
void setcolor(int pairi);
void uncolor(int pairi);

#ifdef USECOLOR
#define NCURSES_BITS(mask,shift) ((mask) << ((shift) + NCURSES_ATTR_SHIFT))
#define A_NORMAL  0L
#define A_ATTRIBUTES  NCURSES_BITS(~(1UL - 1UL),0)
#define A_CHARTEXT  (NCURSES_BITS(1UL,0) - 1UL)
#define A_COLOR   NCURSES_BITS(((1UL) << 8) - 1UL,0)
#define A_STANDOUT  NCURSES_BITS(1UL,8)
#define A_UNDERLINE NCURSES_BITS(1UL,9)
#define A_REVERSE NCURSES_BITS(1UL,10)
#define A_BLINK   NCURSES_BITS(1UL,11)
#define A_DIM   NCURSES_BITS(1UL,12)
#define A_BOLD    NCURSES_BITS(1UL,13)
#define A_PROTECT NCURSES_BITS(1UL,16)
#endif

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
