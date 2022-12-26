/*
 *	Parameter file for NRO word processor
 *
 *	Stephen L. Browning
 *	5723 North Parker Avenue
 *	Indianapolis, Indiana 46220
 *
 *	adapted to ms-dos 10/17/86 pjv
 *
 */
#if defined(unix) || defined(__hpux) || defined(COHERENT) || defined(__FreeBSD__) || defined(linux)
#define UNIX
#endif

#ifndef min
#define min(a,b) (a < b ? a : b)
#endif
#ifndef max
#define max(a,b) (a > b ? a : b)
#endif
#ifndef OK
#define OK 0
#endif
#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define MACRO	 0	/* macro definition */
#define BP	 1	/* begin page	*/
#define BR	 2	/* break	*/
#define CE	 3	/* center	*/
#define FI	 4	/* fill		*/
#define FO	 5	/* footer	*/
#define HE	 6	/* header	*/
#define IN	 7	/* indent	*/
#define LS	 8	/* line spacing	*/
#define NF	 9	/* no fill	*/
#define PL	10	/* page lenght	*/
#define RM	11	/* right margin	*/
#define SP	12	/* line space	*/
#define TI	13	/* temp indent	*/
#define UL	14	/* underline	*/
#define JU	15	/* justify	*/
#define NJ	16	/* no justify	*/
#define M1	17	/* top margin	*/
#define M2	18	/* second top margin	*/
#define M3	19	/* first bottom margin	*/
#define M4	20	/* bottom-most margin	*/
#define BS	21	/* allow/disallow '\b' in output */
#define NE	22	/* need n lines */
#define PC	23	/* page number character */
#define CC	24	/* control character	*/
#define PO	25	/* page offset	*/
#define BO	26	/* bold face	*/
#define EH	27	/* header for even numbered pages	*/
#define OH	28	/* header for odd numbered pages	*/
#define EF	29	/* footer for even numbered pages	*/
#define OF	30	/* footer for odd numbered pages	*/
#define SO	31	/* source file	*/
#define CU	32	/* continuous underline	*/
#define DE	33	/* define macro	*/
#define EN	34	/* end macro definition	*/
#define NR	35	/* set number register	*/

#define UNKNOWN	-1

/*
 *	MAXLINE is set to a value slightly larger
 *	than twice the longest expected input line.
 *	Because of the way underlining is handled, the
 *	input line which is to be underlined, can almost
 *	triple in length.  Unlike normal underlining and
 *	boldfacing, continuous underlining affects all
 *	characters in the buffer, and represents the
 *	worst case condition.  If the distance between
 *	the left margin and the right margin is greater
 *	than about 65 characters, and continuous underlining
 *	is in effect, there is a high probability of buffer
 *	overflow.
 */

#define MAXLINE	200000
#define PAGELEN	 66
#define PAGEWIDTH 80
#define HUGE	256
#define LEFT	0		/* indecies into header margin limit arrays */
#define RIGHT	1
#define NFILES	4		/* nesting depth for input files */

/*
 *	The following parameters may be defined elsewhere
 */

#define YES	1
#define NO	0
#define ERR	-1

/*
 *	The parameter values selected for macro definitions
 *	are somewhat arbitrary.  MACBUF is the storage area
 *	for both macro names and definitions.  Since macro
 *	processing is handled by pushing back the expansion
 *	into the input buffer, the longest possible expansion
 *	would be MAXLINE characters.  Allowing for argument
 *	expansion, MXMLEN was chosen slightly less than MAXLINE.
 *	It is assumed that most macro definitions will not
 *	exceed 20 characters, hence MXMDEF of 100.
 */

#define MXMDEF	400		/* maximum no. of macro definitions */
#define MACBUF	80000		/* macro definition buffer */
#define MXMLEN	180000		/* maximum length of each macro definition */
#define MNLEN	100		/* maximum length of macro name */

struct macros {
	char *mnames[MXMDEF];	/* table of pointers to macro names */
	int lastp;		/* index to last mname	*/
	char *emb;		/* next char avail in macro defn buffer */
	char mb[MACBUF];	/* table of macro definitions */
	char *ppb;		/* pointer into push back buffer */
	char pbb[MAXLINE];	/* push back buffer */
};


/* control parameters for nro */

struct docctl {
	int fill;	/* fill if YES, init = YES		*/
	int lsval;	/* current line spacing, init = 1	*/
	int inval;	/* current indent, >= 0, init = 0	*/
	int rmval;	/* current right margin, init = 60	*/
	int tival;	/* current temp indent, init = 0	*/
	int ceval;	/* number of lines to center, init = 0	*/
	int ulval;	/* number of lines to underline, init = 0 */
	int cuval;	/* no. lines to continuously underline, init = 0 */
	int juval;	/* justify if YES, init = YES		*/
	int boval;	/* number of lines to bold face, init = 0 */
	int bsflg;	/* can output contain '\b', init = FALSE */
	int soflg;	/* soelim mode -o, init = FALSE */
	char pgchr;	/* page number character, init = '#'	*/
	char cmdchr;	/* command character, init = '.'	*/
	int prflg;	/* print on or off, init = TRUE		*/
	int sprdir;	/* direction for spread(), init = 0	*/
	int flevel;	/* nesting depth for source cmd, init = 0 */
	int nr[26];	/* number registers	*/
};


/* output buffer control parameters */

struct cout {
	int outp;	/* next avail char position in outbuf, init = 0 */
	int outw;	/* width of text currently in buffer	*/
	int outwds;	/* number of words in buffer, init = 0	*/
	int lpr;	/* output to printer, init = FALSE	*/
	char outbuf[MAXLINE];	/* output of filled text	*/
};

/* page control parameters for nro */

struct page {
	int curpag;	/* current output page number, init =0	*/
	int newpag;	/* next output page number, init = 1	*/
	int lineno;	/* next line to be printed, init = 0	*/
	int plval;	/* page length in lines, init = 66	*/
	int m1val;	/* margin before and including header	*/
	int m2val;	/* margin after header			*/
	int m3val;	/* margin after last text line		*/
	int m4val;	/* bottom margin, including footer	*/
	int bottom;	/* last live line on page
					= plval - m3val - m4val	*/
	int offset;	/* page offset from left, init = 0	*/
	int frstpg;	/* first page to print, init = 0	*/
	int lastpg;	/* last page to print, init = 30000	*/
	int ehlim[2];	/* left/right margins for headers/footers	*/
	int ohlim[2];	/* init = 0 and PAGEWIDTH			*/
	int eflim[2];
	int oflim[2];
	char ehead[MAXLINE];	/* top of page title, init = '\n'	*/
	char ohead[MAXLINE];
	char efoot[MAXLINE];	/* bottom of page title, init = '\n'	*/
	char ofoot[MAXLINE];
};

char *getmac();
void putlin();
