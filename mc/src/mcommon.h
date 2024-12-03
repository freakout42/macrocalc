/* mcommon.h,v 1.15 2002/10/17 12:25:50 axel */

extern char license;
extern char *progname;
extern char rcsinfo[];
extern char libpath[];
extern char logo[];
extern char filename[MAXFILE+1];
extern char macroname[MAXFILE+1];
extern char macroexec[MAXFILE+1];
extern struct Range *rnames;
extern CELLPTR curcell;
extern unsigned char colwidth[MAXCOLS];
extern unsigned char colstart[MAXCOLS];
extern unsigned char formdisplay;
extern char changed;
extern unsigned char autocalc, borders, cellcont, highlight, menudisp, rdonly, autoexec, markfull;
extern unsigned char colors;
extern unsigned char defaultformat;
extern unsigned char defaultwidth;
extern char currency;
extern int ysiz, xsiz;
extern int screencol, screenlin, leftmargin;
extern int curcelline, inputline, cmdline, errorline, borderline, windowline;
extern int leftcol, rightcol, toprow, topbotrow,
	windowrow, bottomrow, curcol, currow;
extern unsigned char stop;
extern char *msgcommand;
extern int nextkey;
