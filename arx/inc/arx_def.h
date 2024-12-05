/* arx_def.h */

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

#ifdef WIN32
#define ARXPATH "."
#else
#define ARXPATH		"/opt/arx"
#endif

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

#ifndef max
#define max(x, y)	(((x) < (y)) ? (y) : (x))
#define min(x, y)	(((x) < (y)) ? (x) : (y))
#endif

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

#define	CORPORATION	"AxelKReinhold"

#ifndef __PLATFORM__
#define __PLATFORM__ ""
#endif

#if defined(__VERSION__)
#define gnucver " gcc " __VERSION__
#elif defined(__GNUC__)
#define gnucs1(m, i, l) #m "." #i "." #l
#define gnucs(m, i, l) gnucs1(m, i, l)
#define gnucver " gcc " gnucs(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
#define gnucver ""
#endif
#define CCOMPILER gnucver " " __DATE__

#ifdef COHERENT
#define PLATFORM	"COFF-i386"

#elif _M_COFF
#define PLATFORM	"COFF-i386"

#elif __hpux
#define PLATFORM	"HP-UX"

#elif __FreeBSD__
#define PLATFORM	"FreeBSD" __PLATFORM__
/* __PLATFORM__ */

#elif linux
#ifdef OSINFO
#define PLATFORM	OSINFO
#else
#ifdef __ELF__
#ifdef __GLIBC__
#define glibcs1(a, i) " glibc " #a "." #i
#define glibcs(a, i) glibcs1(a, i)
#define PLATFORM "Linux" __PLATFORM__ glibcs(__GLIBC__, __GLIBC_MINOR__)
#else
#define PLATFORM	"Linux ELF"
#endif
#else
#define PLATFORM	"Linux"
#endif
#endif

#elif sparc
#define PLATFORM	"Solaris"

#elif _AIX
#define PLATFORM	"AIX"

#elif __DARWIN_UNIX03
#define PLATFORM	"MacOSX"

#elif WIN32
#define PLATFORM	"Windows"

#else
#define PLATFORM	"Unknown"
#endif

#define	COPYRIGHT	PLATFORM " (c)" "2024" " " CORPORATION
#if defined(unix) || defined(__unix) || defined(__hpux) || defined(COHERENT) || defined(__FreeBSD__) || defined(linux) || defined(_AIX) || defined(__DARWIN_UNIX03)
#define UNIX
#endif

int lib_akey (char *logo, int show);
char *lib_veid (void);
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

#if defined(_STDIO_H) || defined(_STDIO_INCLUDED) || defined(__STDIO_H__) || defined(_STDIO_H_) || defined(_H_STDIO) || defined(WIN32)

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

#if defined(MSDOS) || defined(WIN32)
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

#if defined(__FreeBSD__) || defined(linux) || defined(sparc) || defined(_AIX) || defined(__SVR4) || defined(__DARWIN_UNIX03) || defined(WIN32)
#define DIRSIZ 255
#else
#include <sys/dir.h>
#endif

#define MAXPATH	1024		/* Size of static pathname buffer */
extern char *lib_path (char *path, char *file, int mode);

#endif

#if defined(_TIME_H) || defined(_TIME_H_)
time_t lib_mktm (struct tm *tp);
#endif
