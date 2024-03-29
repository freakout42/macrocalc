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
#define SCREENROWS	(screenlin-(cellcont&&menudisp ? 4 : 3))
#define DEFAULTWIDTH	11
#define MARKPOS		55
#define FORMPOS		67
#define AUTOPOS		72

#define UNRECUNIT	64
#define CONFUNIT	65
#define UNRECRANGE	66

enum	{
	EMPTY,      /* =0     new empty */
	TEXT,       /* =1  yy literal with just [0]=['"] */
	CONSTANT,   /* =2  yy default before yyparse */
	FORMULA,    /* =3  yy */
	UNITT,      /* =4     sidecar cell */
	SYNERROR,   /* =5  yy temporary returned by parse */
	INCOMMAND,  /* =6  yy */
	OUTCOMMAND, /* =7  yy */
	RETRIEVED,  /* =8  |< literal from pipe */
	VRETRIEVED, /* =9  |< value from pipe */
	EOFPIPE,    /* =10 |< eof from pipe */
	STRING,     /* =11 yy string formula */
	DATETYPE,	  /* =12 yy only temporary used for date format */
	COMPLEX,    /* =13    not used should use CONSTANT|FORMULA */
	COLWIDTH,   /* =14    only for fileio coldef was =0 */
	ERRORT      /* =15    not used */
	};

#define EPOCH		25569.
#define ROLLYEAR	70.

#define STRLEFT		'\''
#define STRRIGHT	'\"'
#define STRCENTER	'^'
#define STRREPEAT	'\\'

#define ATTRIBM	 0xcfu
#define BIMASK	 0xc0u
#define TYPEM    0x1fu
#define COLORM   0x0fu
#define UNITF	   0x20u /* has sidecar */
#define BOLD     0x40u
#define ITALIC   0x80u

#define PROTECT    0x80u
#define FORMATM    0x70u
#define SPECIAL    0x00u
#define SCIENTIFIC 0x10u
#define CURRENCY   0x20u
#define PERCENT    0x30u
#define COMMA      0x40u
#define FIXED      0x50u
#define PLACES     0x0fu

#define L_FIXED    0x00u
#define L_SPECIAL  0x70u
#define L_DEFAULT  0x0fu

#define DEFAULT    0x00u
#define GENERAL    0x01u
#define DATE       0x02u
#define DAYMONTH   0x03u
#define MONTHYEAR  0x04u
#define TEXTF      0x05u
#define HIDDEN     0x06u
#define TIME       0x07u
#define HOURMIN    0x08u
#define DATEI1     0x09u
#define EDATE      0x09u
#define DATEI2     0x0au
#define RDBDATE    0x0au
#define TIMEI1     0x0bu
#define TIMEI2     0x0cu
#define BAR        0x0du

#define DEFAULTFORMAT	  ((0x00u & PROTECT) | (FORMATM & SPECIAL)   | (PLACES & DEFAULT))
#define L_DEFAULTFORMAT	((0x00u & PROTECT) | (FORMATM & L_SPECIAL) | (PLACES & L_DEFAULT))

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

#define	cpnext(cp)    ((cp)->next)
#define	cpadr(cp)     ((cp)->adr)
#define	cpcol(cp)     (((cp)->adr).col)
#define	cprow(cp)     (((cp)->adr).row)
#define	cptype(cp)    ((cp)->type)
#define	cpval(cp)     ((cp)->v)
#define	cpvalue(cp)   (((cp)->v).value)
#define	cpcimag(cp)   (((cp)->v).cimag)
#define	cpiscmplx(cp) (fabs(((cp)->v).cimag)>0.0001)
#define	cpunit(cp)    (((cp)->v).unit)
#define	cplength(cp)  (((cp)->s).length)
#define	cpstring(cp)	(((cp)->s).string)
#define	cpattrib(cp)  ((cp)->attrib)
#define	cpattrbi(cp)  ((cp)->attrib & BIMASK)
#define	cpcolor(cp)   ((cp)->attrib & COLORM)
#define	cpsidecar(cp) ((cp)->attrib & UNITF) /* has sidecar */
#define	cpneedsid(cp) ((cp)->type != UNITT   && (cpunit(cp) || cpcimag(cp)))
#define	cpformula(cp) ((cp)->type == FORMULA || cptype(cp) == STRING)
#define	cpnumber(cp)  ((cp)->type == FORMULA || cptype(cp) == CONSTANT || cptype(cp) == VRETRIEVED || cptype(cp) == COMPLEX)
#define	cpfor(cp)     ((cp)->format)
#define	cpprotect(cp) ((cp)->format & PROTECT)
#define	cpformat(cp)  ((cp)->format & FORMATM)
#define	cpform(cp)    ((cp)->format & (FORMATM|PLACES))
#define	cpplaces(cp)  ((cp)->format & PLACES)
#define	cptext(cp)    ((cp)->text)
#define	cpliteral(cp) (cptype(cp) == TEXT)
#define	cptextstr(cp) (cpliteral(cp)?(cp)->text+1:(cp)->text)
