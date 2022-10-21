/* $Id: arx_def.h,v 1.54 2020/12/07 09:55:19 axel Exp $
 * $Log: arx_def.h,v $
 * Revision 1.54  2020/12/07 09:55:19  axel
 * oem2iso
 *
 * Revision 1.53  2017/07/11 15:01:33  axel
 * arxpath=/opt/arx
 *
 * Revision 1.52  2016/11/07 08:15:32  axel
 * 2017
 *
 * Revision 1.51  2016/10/31 18:27:49  axel
 * amd64
 *
 * Revision 1.50  2015/02/04 11:34:13  axel
 * adsfix with clipper
 *
 * Revision 1.49  2012/02/21 15:09:31  axel
 * 2012
 *
 * Revision 1.48  2012/02/20 20:00:35  axel
 * MacOSX
 *
 * Revision 1.47  2006/09/28 09:43:58  axel
 * solaris x86
 *
 * Revision 1.46  2006/09/27 06:22:21  axel
 * ident logos
 *
 * Revision 1.45  2006/09/26 13:43:35  axel
 * improved idents
 *
 * Revision 1.44  2006/09/25 15:46:59  axel
 * FreeBSD license check
 *
 * Revision 1.43  2006/07/06 13:15:42  axel
 * glibc version
 *
 * Revision 1.42  2005/08/03 17:30:35  axel
 * v406 2005
 *
 * Revision 1.41  2004/12/31 14:12:12  axel
 * yywrap lib for AIX dynamic now
 *
 * Revision 1.40  2004/04/12 11:16:08  axel
 * aix
 *
 * Revision 1.39  2004/01/20 19:06:43  axel
 * 2004
 *
 * Revision 1.38  2003/06/11 19:11:05  axel
 * Solaris 8
 *
 * Revision 1.37  2002/09/07 12:38:01  axel
 * *** empty log message ***
 *
 * Revision 1.36  2001/11/07 12:24:15  axel
 * icc port (not db2rdb, mc2wks due to alignment troubles)
 *
 * Revision 1.35  2001/06/06 09:27:15  axel
 * 2001
 *
 * Revision 1.34  1999/10/15 17:21:00  axel
 * SCO OS5 cc
 *
 * Revision 1.33  1997/04/11 08:03:16  axel
 * 1997
 *
 * Revision 1.32  1997/04/06  10:58:46  axel
 * lib_mktm
 *
 * Revision 1.31  1996/10/26  13:15:52  axel
 * maxpath 1024
 *
 * Revision 1.30  1996/10/20  16:31:15  axel
 * new year 1996
 *
 * Revision 1.29  1995/06/24  12:03:39  axel
 * Linux
 *
 * Revision 1.28  1995/06/11  18:09:27  axel
 * define UNIX
 *
 * Revision 1.27  1995/06/09  05:12:19  axel
 * gets() macro to force usage of fgets()
 *
 */

#ifdef PRODUCTION
#define NDEBUG
#endif

#ifndef EXIT_SUCCESS
#define	EXIT_SUCCESS	0	/* successful termination */
#define	EXIT_FAILURE	1	/* operation was not successful */
#endif

#define	EXIT_WARNING	2	/* successful, but questionable outcome */
#define	EXIT_SYNTAX	2	/* syntax error in command */
#define	EXIT_FATAL	3	/* a serious error occurred */
#define	EXIT_VIOLATE	127	/* a copyright violation */

#define	RET_SUCCESS	0	/* successful termination */
#define	RET_ERROR	1	/* operation was not successful */
#define	RET_WARNING	2	/* successful, but questionable outcome */
#define	RET_FATAL	3	/* a serious error occurred */

#define ARXPATH		"/opt/arx"

#define	WHITESPACE	char *whitespace = " \t\n\r\f"

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#define	CHILD	0
#define PARENT	1
#define FAIL	-1
#define READ	0
#define WRITE	1

#define max(x, y)	(((x) < (y)) ? (y) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))

#if defined(i386) || defined(_I386) || defined(_M_IX86) || defined(__x86_64)
#define lib_cano(v)
#define lib_2iee(d)	d
#define lib_iee2(d)	d

#elif defined(__hppa) || defined(sparc) || defined(_POWER)
#define lib_cano(v) swab(&v, &v, sizeof(v))
double lib_2iee (double d);
double lib_iee2 (double d);

#endif

/* __DATE__ parts */
#define	MONTH	0
#define	MONTHLN	3
#define	DAY	4
#define	DAYLN	2
#define	YEAR	7
#define	YEARLN	4

#define	CORPORATION	"/ARX"
#define SERIALNO	"Serialno: 000000"
#define ARXLICENSE	1

#ifdef __GNUC__
#define CCOMPILER	" gcc " __VERSION__
#else
#define CCOMPILER	""
#endif

#ifdef COHERENT
#define PLATFORM	"COFF-i386"

#elif _M_COFF
#define PLATFORM	"COFF-i386"

#elif __hpux
#define PLATFORM	"HP-UX"

#elif __FreeBSD__
#define PLATFORM	"FreeBSD " __PLATFORM__
/* __PLATFORM__ */
#undef ARXLICENSE

#elif linux
#ifdef OSINFO
#define PLATFORM	OSINFO
#else
#ifdef __ELF__
#ifdef __GLIBC__
#define glibcs1(a, i) " glibc " #a "." #i
#define glibcs(a, i) glibcs1(a, i)
#define PLATFORM "Linux " __PLATFORM__ glibcs(__GLIBC__, __GLIBC_MINOR__)
#else
#define PLATFORM	"Linux ELF"
#endif
#else
#define PLATFORM	"Linux"
#endif
#endif
#undef ARXLICENSE

#elif sparc
#define PLATFORM	"Solaris"

#elif _AIX
#define PLATFORM	"AIX"

#elif __DARWIN_UNIX03
#define PLATFORM	"MacOSX"

#else
#define PLATFORM	"ELF-i386"
#endif

#define	COPYRIGHT	PLATFORM " (c)" "2020" " " CORPORATION " " SERIALNO
#if defined(unix) || defined(__unix) || defined(__hpux) || defined(COHERENT) || defined(__FreeBSD__) || defined(linux) || defined(_AIX) || defined(__DARWIN_UNIX03)
#define UNIX
#endif

int lib_akey (char *logo, int show);
void lib_fatl (int s, char *msg, ...);
void lib_mesg (char *msg, ...);

#ifdef COHERENT
/* Constants. now defined in math.h */
#define M_PI            0.31415926535897932e+01
#define SQRT2           0.14142135623730950e+01
#define LOG2B10         0.30102999566398119e+00
#define LOG10BE         0.23025850929940456e+01
#define LOG10B2         0.33219280948873623e+01
#define LOGEB2          0.14426950408889634e+01
#endif

#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(__STDIO_H__) || defined(_STDIO_H_) || defined(_H_STDIO)

/* force usage of fgets() instead of dangerous gets() */
#define gets(buf) fgets(buf, sizeof(buf), stdin)

struct file
	{
	char	*name;
	short	mode;
	int	gid;
	char	*grp;
	int	uid;
	char	*own;
	size_t	size;
	struct tm *time;
	};

FILE *lib_freo (char *filename, char *mode, FILE *stream);
int lib_twpo (FILE **in, FILE **out, char *cmd);
struct file *lib_stat (char *f);

#ifndef _NFILE
#define _NFILE FOPEN_MAX
#endif

#ifndef STDIN_FILENO
#define	STDIN_FILENO	0
#define	STDOUT_FILENO	1
#define	STDERR_FILENO	2
#endif

#ifdef UNIX
#define PATHSEP		'/'	/* Path name component separator */
#define	PATHSEPSTRING	"/"
#define LISTSEP		':'	/* Search list component separator */
#define DEFPATH		":/bin:/usr/bin"
#define DEFLIBPATH	"/lib:/usr/lib"
#define DEFSHELL	"sh"
#endif

#ifdef GEMDOS
#define PATHSEP		'\\'
#define	PATHSEPSTRING	"\\"
#define LISTSEP		','
#define DEFPATH		",\\bin,\\usr\\bin"
#define DEFLIBPATH	"\\lib,,\\usr\\lib"
#define DEFSHELL	"msh.prg"
#endif

#ifdef MSDOS
#define PATHSEP		'\\'
#define	PATHSEPSTRING	"\\"
#define LISTSEP		';'
#define DEFPATH		".;c:\\bin;c:\\usr\\bin"
#define DEFLIBPATH	"\\lib;c:\\usr\\lib"
#define	DEFSHELL	getenv("COMSPEC")	/* was "command.com" */
#endif

#ifdef _SCO_DS
#include <unistd.h>
#endif

#if defined(__FreeBSD__) || defined(linux) || defined(sparc) || defined(_AIX) || defined(__SVR4) || defined(__DARWIN_UNIX03)
#define DIRSIZ 255
#else
#include <sys/dir.h>
#endif

#define MAXPATH	1024		/* Size of static pathname buffer */
extern char *lib_path (char *path, char *file, int mode);

#endif

#ifdef _TIME_H
time_t lib_mktm (struct tm *tp);
#endif
