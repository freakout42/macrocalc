/* mcelldef.h 1.21 2016/10/31 06:35:14 axel */

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
	UNITT, /* the side cell */
	SYNERROR,
	INCOMMAND,
	OUTCOMMAND,
	RETRIEVED,
	VRETRIEVED,
	EOFPIPE,
	STRING,
	DATETYPE,	/* only temporary used for date format */
	EMPTY
	};

#define EPOCH		25569.
#define ROLLYEAR	70.

#define STRLEFT		'\''
#define STRRIGHT	'\"'
#define STRCENTER	'^'
#define STRREPEAT	'\\'

#define	ATTRIBM		0xc0u
#define	TYPEM		0x1fu
#define	BOLD		0x40u
#define	ITALIC		0x80u
#define UNITF		0x20u

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

struct CELLADR
	{
	int	col;
	int	row;
	};

struct CELLVAL {
  int type;
  union CELLUNION {
    struct CELLVALUE {
      double value;
      double cimag;
      char *unit;
    } v;
    struct CELLSTRING {
      short length;
      char *string;
    } s;
  } u;
};

#define	cpv(cp)		(cp->val)
#define	cpval(cp)	((cp->val)->u.v)
#define	cpvalue(cp)	(cp->val?(cp->val)->u.v.value:0.)
#define	lcpvalue(cp)	((cp->val)->u.v.value)
#define	cpu(cp)		(cp->val?(cp->val)->u.v.unit:NULL)
#define	lcpu(cp)	((cp->val)->u.v.unit)
#define	cplength(cp)	(cp->val?(cp->val)->u.s.length:0)
#define	lcplength(cp)	((cp->val)->u.s.length)
#define	cpstring(cp)	(cp->val?(cp->val)->u.s.string:NULL)
#define	lcpstring(cp)	((cp->val)->u.s.string)
#define	cpunit(cp)	(cp->attrib & UNITF ? (cp->val)->u.v.unit : NULL)
#define	cpatt(cp)	(cp->attrib)
#define	cpattrib(cp)	(cp->attrib & ATTRIBM)
#define	cpunitf(cp)	(cp->attrib & UNITF)
#define	cptype(cp)	(cp->attrib & TYPEM)
#define	cpformula(cp)	(cptype(cp) == FORMULA || cptype(cp) == STRING)
#define	cpnumber(cp)	(cptype(cp) == FORMULA || cptype(cp) == CONSTANT || cptype(cp) == VRETRIEVED)
#define	cpprotect(cp)	(cp->format & PROTECT)
#define	cpfor(cp)	(cp->format)
#define	cpformat(cp)	(cp->format & FORMATM)
#define	cpform(cp)	(cp->format & (FORMATM|PLACES))
#define	cpplaces(cp)	(cp->format & PLACES)
#define	cptext(cp)	(cp->text)
#define	cptextstr(cp)	(cpnumber(cp)?cp->text:cp->text+1)

struct CELLREC
	{
	struct CELLREC	*next;
	struct CELLADR	adr;
	unsigned char	attrib;
	unsigned char	format;
	char		*text;
	struct CELLVAL *val;
	};

typedef struct CELLREC *CELLPTR;

#define adrval(col) (col>=0?col:-col-1)

struct Range
	{
	struct Range	*next;
	struct CELLADR	adr[2];
	char		*name;
	};
