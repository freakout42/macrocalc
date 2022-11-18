/* mcelldef.h */

#define MAXCOLS		27*26 /* A-Z AA-ZZ */
#define MAXROWS		99999
#define MAXADR		8
#define MAXINPUT	32767
#define MAXFILE		255
#define MAXPRINT	MAXINPUT
#define MAXPARSED	MAXINPUT
#define MAXPLACES	16

#define	SCREENWIDTH	screencol
#define MAXLEFTMARGIN	5
#define LEFTMARGIN	leftmargin
#define MINCOLWIDTH	0
#define	INPUTWIDTH	SCREENWIDTH
#define MAXCOLWIDTH	(SCREENWIDTH - LEFTMARGIN)
#define SCREENCOLS	(SCREENWIDTH - LEFTMARGIN) / MINCOLWIDTH + 1
#define SCREENROWS	(screenlin-4)
#define DEFAULTWIDTH	11
#define MARKPOS		55
#define FORMPOS		67
#define AUTOPOS		72

#define UNRECUNIT	64
#define CONFUNIT	65
#define UNRECRANGE	66

enum	{
	COLWIDTH,
	TEXT,
	CONSTANT,
	FORMULA,
	UNITT, /* sidecar cell */
	SYNERROR,
	INCOMMAND,
	OUTCOMMAND,
	RETRIEVED,
	VRETRIEVED,
	EOFPIPE,
	STRING,
	DATETYPE,	/* only temporary used for date format */
	EMPTY,
	COMPLEX,
	ERRORT
	};

#define EPOCH		25569.
#define ROLLYEAR	70.

#define STRLEFT		'\''
#define STRRIGHT	'\"'
#define STRCENTER	'^'
#define STRREPEAT	'\\'

#define	BOLD		0x40u
#define	ITALIC		0x80u

#define PROTECT		0x80u
#define FORMATM		0x70u
#define FIXED		0x00u
#define SCIENTIFIC	0x10u
#define CURRENCY	0x20u
#define PERCENT		0x30u
#define COMMA		0x40u
#define SPECIAL		0x70u
#define PLACES		0x0fu
#define BAR		0x00u
#define GENERAL		0x01u
#define DATE		0x02u
#define DAYMONTH	0x03u
#define MONTHYEAR	0x04u
#define TEXTF		0x05u
#define HIDDEN		0x06u
#define TIME		0x07u
#define HOURMIN		0x08u
#define DATEI1		0x09u
#define EDATE		0x09u
#define DATEI2		0x0au
#define RDBDATE		0x0au
#define TIMEI1		0x0bu
#define TIMEI2		0x0cu
#define DEFAULT		0x0fu
#define DEFAULTFORMAT	((0&PROTECT) | (FORMATM&SPECIAL) | (PLACES&DEFAULT))

struct CELLADR {
  int	col;
  int	row;
  };
struct CELLVALUE {
  double value;
  double cimag;
  char  *unit;
  };
struct CELLSTRING {
  short length;
  char *string;
  };

struct CELLREC {
  struct CELLADR adr;
  struct CELLREC *next;
  unsigned char type;
  unsigned char attrib;
  unsigned char format;
  char *text;
  struct CELLVALUE v;
  struct CELLSTRING s;
  };
typedef struct CELLREC cellr;
typedef struct CELLREC *CELLPTR;

#define adrval(col) (col>=0?col:-col-1)

struct Range {
  struct Range *next;
  struct CELLADR adr[2];
  char *name;
  };

#define	cpnext(cp)    (cp->next)
#define	cpcol(cp)     ((cp->adr).col)
#define	cprow(cp)     ((cp->adr).row)
#define	cptype(cp)    (cp->type)
#define	cpvalue(cp)   ((cp->v).value)
#define	cpcimag(cp)   ((cp->v).cimag)
#define	cpunit(cp)    ((cp->v).unit)
#define	cplength(cp)  ((cp->s).length)
#define	cpstring(cp)	((cp->s).string)
#define	cpattrib(cp)  (cp->attrib)
/*efine	cpformula(cp) (cptype(cp) == FORMULA || cptype(cp) == STRING)*/
/*efine	cpnumber(cp)  (cptype(cp) == FORMULA || cptype(cp) == CONSTANT || cptype(cp) == VRETRIEVED)*/
#define	cpprotect(cp) (cp->format & PROTECT)
#define	cpfor(cp   )  (cp->format)
#define	cpformat(cp)  (cp->format & FORMATM)
#define	cpform(cp)    (cp->format & (FORMATM|PLACES))
#define	cpplaces(cp)  (cp->format & PLACES)
#define	cptext(cp)    (cp->text)
#define	cpliteral(cp) (cptype(cp) == TEXT)
#define	cptextstr(cp) (cpliteral(cp)?cp->text+1:cp->text)
