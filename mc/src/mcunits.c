/* mcunits.c
 * units -- do multiplicative unit conversions
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <arx_def.h>

#define	PATHSIZE 128
#define	NDIM	12
#define	NUNITS	3000
#define	NNAMES	3000		/* unit names */
#define	ANAME	16		/* name space */
#define	NBUF	256		/* Length of longest line */
#define	UMAGIC	0123456		/* Magic number written in binary header */

/*
 * Header for the units file.
 * This  tells that it is the real
 * thing and how much to read in.
 */
#ifndef MAINUNIT
#include "binunits.h"
#define printf nullprintf
static void nullprintf(char *x, ...) {}
#endif
typedef	struct	HDR {
	unsigned h_magic;
	unsigned h_nunits;
	unsigned h_ssize;		/* String space size */
}	HDR;

HDR	hdr;

typedef	struct	UNIT {
	char		*u_name;
	double		u_val;
	signed char	u_dim[NDIM];
}	UNIT;

UNIT	*units;
struct	stat	sb;
char	ufile[] = "lib/mcunits.rdb";
char	binufile[] = "lib/binunits";
char	*ufname;
char	*bufname;
char	buf[NBUF];
char	inbuf[BUFSIZ];

int nunits;
int uflag = 0;			/* Update information only */
int dflag = 0;			/* Debug to stderr */
FILE *fd;
int peekc = EOF;
int lastc = EOF;
int lineno = 1;

struct{
	char *prefix;
	double factor;
}pre[]={
{"femto",	1e-15},
{"pico",	1e-12},
{"nano",	1e-9},
{"micro",	1e-6},
{"milli",	1e-3},
{"centi",	1e-2},
{"deci",	1e-1},
{"hemi",	.5},
{"demi",	.5},
{"semi",	.5},
{"sesqui",	1.5},
{"deka",	1e1},
{"hecto",	1e2},
{"hekto",	1e2},	/* common (?) misspelling */
{"kilo",	1e3},
{"myria",	1e5},
{"mega",	1e6},
{"giga",	1e9},
{"tera",	1e12},
{NULL,		1.}
};

/*
 * Return the string stripped of its
 * prefix (if any).  Set factor
 * to the multiplicative factor indicated by
 * the prefix found.
 */
char *prefix (char *str, double *factor)
{
	register char *s, *t;
	register int i;

	for (i=0; pre[i].prefix!=NULL; i++) {
		s = pre[i].prefix;
		t = str;
		while (*s != '\0')
			if (*s++ != *t++)
				break;
		if (*s == '\0') {
			*factor = *factor * pre[i].factor;
			return (t);
		}
	}
	return(NULL);
}

/*
 * Check for any occurrences of
 * the character `c' in string `s'.
 */
int anyc (char c, char *s)
{
	while (*s != '\0')
		if (c == *s++)
			return (1);
	return (0);
}

/*
 * Return non-zero if string `s' is the
 * same or plural as string `t'.
 */
int eqplural (char *s, char *t)
{
	while (*t != '\0')
		if (*s++ != *t++)
			return (0);
	return (*s=='\0' || (*s++=='s' && *s=='\0'));
}

#ifdef MAINUNIT
/*
 * Diagnostics
 */
void cerr (char *x, ...)
{
	va_list	args;
	va_start (args, x);
	fprintf (stderr, "units: ");
	vfprintf (stderr, x, args);
	fprintf (stderr, "\n");
	va_end (args);
	exit(1);
}

/*
 * get contiguous memory.
 */
char *alloc (unsigned int nb)
{
	register char *rp;
	if((rp = malloc(nb)) == NULL)
		cerr("out of memory");
	return (rp);
}
#endif

/*
 * allocate a new copy.
 */
char *newcpy (char *s)
{
	return(strcpy(malloc(strlen(s) + 1), s));
}

#ifdef MAINUNIT
/*
 * find a file on a path in the environment, or a default path
 * with an access priveledge.
 *
 * example: pathn("helpfile", "LIBPATH", ",,\lib", "r");
 *
 * Returns full path name.
 */
char *pathn (char *name, char *envpath, char *deflpath, char *acs)
{
	static char fullname[PATHSIZE];
	register char *pathptr;

	if((NULL == envpath) || (NULL == (pathptr = getenv(envpath))))
		pathptr = deflpath;

	if(NULL == strchr(acs, 'w')) {
		if ((pathptr = lib_path(pathptr, name, R_OK)) == NULL)
			fullname[0] = '\0'; /* bad name */
		else
			strcpy(fullname, pathptr);
	}
	else {
		register char *p, c;

		if((p = lib_path(pathptr, name, W_OK)) == NULL) {
			for(p = fullname; (c = *pathptr++) && c != LISTSEP;)
				*p++ = c;
			*p++ = PATHSEP;
			strcpy(p, name);
		} else
			strcpy(fullname, p);
	}
	return(fullname);
}
#endif

/*
 * Attempt to read in the already-stored
 * binary information.  Return non-zero if
 * successful.
 *
 * if(binary and ascii)
 *	 if(binary out of date)
 *		 rebuild
 *	 else
 *		use binary
 * if(binary and no ascii)
 *	use binary
 * if(ascii and no binary)
 *	rebuild
 * crash
 */
int binary (void)
{
	register char *sstart;
	register int n;

#ifdef MAINUNIT
	register int bfd;
	time_t timeasc;

	ufname  = newcpy(pathn(ufile, "ARX", ARXPATH, "r"));
	bufname = newcpy(pathn(binufile, "ARX", ARXPATH, "r"));
	if(!bufname[0]) { /* no binary */
		if(!ufname[0]) /* also no ascii */
			cerr("can't find unit file `%s' or `%s'", 
				ufile, binufile);
		bufname = malloc(strlen(ufname) + strlen(binufile));
		strcpy(bufname, ufname);
		strcpy(strchr(bufname, '\0') - strlen(ufile), binufile);
		return(0);
	}
	else if(!stat(ufname, &sb)) { /* binary and ascii found */
		timeasc = sb.st_mtime;
		if(stat(bufname, &sb) || (timeasc > sb.st_mtime))
			return(0);	/* bin file out of date */
	}
	if(uflag)	/* update only */
		return(0);
	if((bfd = open(bufname, O_BINARY)) < 0)
		return (0);
	if(read(bfd, &hdr, sizeof(hdr)) != sizeof(hdr))
		goto bad;
#else
  memcpy(&hdr, hdrbuf, sizeof(hdr));
#endif
	if(hdr.h_magic != UMAGIC)
		goto bad;
	nunits = hdr.h_nunits;
	sstart = malloc(hdr.h_ssize);
#ifdef MAINUNIT
	if (read(bfd, sstart, hdr.h_ssize) != hdr.h_ssize)
		goto bad;
#else
  memcpy(sstart, hdrbuf+sizeof(hdr), hdr.h_ssize);
#endif
	units = (UNIT *)malloc(n = nunits*sizeof(UNIT));
#ifdef MAINUNIT
	if (read(bfd, units, n) != n)
		goto bad;
#else
  memcpy(units, hdrbuf+(sizeof(hdr)+hdr.h_ssize), n);
#endif
	for (n=0; n!=nunits; n++)
		{
		units[n].u_name += (long)sstart;
		if (dflag && units[n].u_name != NULL) {
			fprintf (stderr, "u_name=%s u_val=units[%03d].u_val=%f\n",
				units[n].u_name, n, units[n].u_val);
			}
		}
#ifdef MAINUNIT
	close(bfd);
#endif
	return (1);
bad:
#ifdef MAINUNIT
	close(bfd);
#endif
	return (0);
}

int nextc (void)
{
#ifndef MAINUNIT
  if (!(lastc = *bufname++)) lastc = '\n';
#else
	register int c;

	if (peekc != EOF) {
		c = peekc;
		peekc = EOF;
		return (c);
	}
	if (lastc == '\n')
		lineno++;
	lastc = getc(fd);
	if (dflag) fputc(lastc, stderr);
	if (lastc == '#') {	/* Eat a comment */
		do {
			lastc = getc(fd);
		} while(lastc!='\n' && lastc!=EOF);
	}
#endif
	return (lastc);
}

double ipow (double d, int n)
{
	double v;

	v = 1.;
	if (n < 0) {
		d = 1./d;
		n = -n;
	}
	while (n) {
		v *= d;
		--n;
	}
	return (v);
}

int getunit (UNIT *u, char *prompt)
{
	register int c;
	register char *s;
	register int i;
	int j, expon, digit, div, pow;
	double factor;

#ifdef MAINUNIT
Again:
	if (prompt != NULL) printf("%s", prompt);
	if (dflag) fprintf(stderr, "%s", prompt);
#else
  bufname = prompt;
#endif
	u->u_val = 1.;
	for (i=0; i != NDIM; i++)
		u->u_dim[i] = 0;
	div = 0;
	pow = 1;
	for(;;)switch(c=nextc()){
	case ' ':
	case '\t':
		break;
	case '\n':
		return (1);
	case EOF:
		return (0);
	case '0':case '1':case '2':case '3':case '4':
	case '5':case '6':case '7':case '8':case '9':
	case '.':case '-':case '+':
		/*
		 * a palpable number
		 */
		s = buf;
		if (c == '+')
			c = nextc();
		digit = 0;
		while (c>='0' && c<='9') {
			*s++ = c;
			c = nextc();
			digit++;
		}
		if (c == '.') {
			*s++ = c;
			while ((c=nextc())>='0' && c<='9') {
				*s++ = c;
				digit++;
			}
		}
		if (!digit) {
		Badnumber:
			*s = '\0';
			printf("Bad number `%s'\n", buf);
			goto Bad;
		}
		if (c=='e' || c=='E') {
			*s++ = 'e';
			c = nextc();
			if (c == '+')
				c = nextc();
			else if (c == '-') {
				*s++ = c;
				c = nextc();
			}
			if (c<'0' || '9'<c)
				goto Badnumber;
			do {
				*s++ = c;
				c = nextc();
			} while('0'<=c && c<='9');
		}
		*s = '\0';
		peekc = c;
		factor = atof(buf);
		if (div) {
			if (factor == 0.) {
				printf("Divide check\n");
				goto Bad;
			}
			u->u_val /= factor;
			div = 0;
		} else
			u->u_val *= factor;
		break;

	case '/':	/* divide by next unit */
		if (div) {
		Baddiv:
			printf("Two division signs in a row\n");
			goto Bad;
		}
		div++;
		break;

	case '!':	/* primitive unit */
		i = 0;
		if ((c = nextc())<'0' || c>'9') {
			printf("`!' must precede a number\n");
			goto Bad;
		}
		do {
			i = i*10+c-'0';
			c = nextc();
		} while('0'<=c && c<='9');
		peekc = c;
		if (i<0 || NDIM<=i) {
			printf("Primitive unit out of range [0,%d]\n", NDIM-1);
			goto Bad;
		}
		u->u_dim[i]++;
		break;

	default:
		s = buf;
		do {
			*s++ = c;
			c = nextc();
		} while(c!=EOF && !anyc(c, "/0123456789+-. \t\n"));
		*s = '\0';
		s = buf;
		if (strcmp(s, "per") == 0) {
			if (div)
				goto Baddiv;
			div++;
			break;
		}
		if (strcmp(s, "square")==0 || strcmp(s, "sq")==0) {
			pow *= 2;
			break;
		}
		if (strcmp(s, "cubic")==0 || strcmp(s, "cu")==0) {
			pow *= 3;
			break;
		}
		factor = 1.;
		do {
			for (i=0; i!=nunits; i++)
				if (eqplural(s, units[i].u_name))
					break;
		} while(i==nunits && (s=prefix(s, &factor))!=NULL);
		if (i == nunits) {
			printf("Unrecognised unit %s\n", buf);
			goto Bad;
		}
		if (c=='+' || c=='-') {
			if (c == '-')
				div = !div;
			expon = 0;
			if ((c = nextc())<'0' || c>'9') {
				printf("+ or - must be followed by digits\n");
				goto Bad;
			}
			do {
				expon = expon*10+c-'0';
				c = nextc();
			} while('0'<=c && c<='9');
		} else
			expon = 1;
		expon *= pow;
		pow = 1;
		peekc = c;
		if (div) {
			expon = -expon;
			div = 0;
		}
		if (dflag) {
			fprintf (stderr, "u_name=%s\n", units[i].u_name);
			fprintf (stderr, "factor=%f units[i].u_val=%f expon=%d\n",
				factor, units[i].u_val, expon);
			}
		u->u_val *= ipow(factor*units[i].u_val, expon);
		for (j=0; j!=NDIM; j++)
			u->u_dim[j] += units[i].u_dim[j]*expon;
	}
Bad:
#ifdef MAINUNIT
	while (c!='\n' && c!=EOF)
		c = nextc();
	if (prompt!=NULL)
		goto Again;
	printf("line %d\n", lineno);
#endif
	return (1);
}

#ifdef MAINUNIT
char *getname (b)
	char *b;
{
	register char *s, *t;
	register int c;

	do {
		c = nextc();
	} while(c==' ' || c=='\n' || c=='\t');
	s = buf;
	while(c!=' ' && c!='\t' && c!='\n' && c!=EOF) {
		*s++ = c;
		c = nextc();
	}
	*s = '\0';
	peekc = c;
	t = b;
	s = buf;
	while ((*t++ = *s++))
		;
	return (t);
}

/*
 * Update units information by reading the
 * units file.
 */
void update (void)
{
	register char *name;
	register int i;
	register char *sstart, *send;
	register int bfd;

	printf("Rebuilding %s from %s ...\n", bufname, ufname);
	if ((fd = fopen(ufname, "rb")) == NULL)
		cerr("can't open unit file `%s'", ufile);
	setbuf(fd, inbuf);
	units = (UNIT *)alloc(NUNITS*sizeof(UNIT));
	sstart = send = alloc(NNAMES*ANAME);
	for (nunits=0; nunits!=NUNITS; nunits++) {
		name = send;
		send = getname(send);
		assert(sstart <= name);
		assert(send-sstart < NNAMES*ANAME-ANAME);
		for (i=0; i!=nunits; i++)
			if (strcmp(units[i].u_name, name) == 0)
				printf("`%s' redefined, line %d\n",
					name, lineno);
		units[nunits].u_name = name;
		if (!getunit(&units[nunits], NULL))
			break;
	}
	if (!feof(fd))
		cerr("too many units");
	fclose(fd);
	/*
	 * Write out, if possible, binary
	 * information for faster response next time.
	 */
	if ((bfd = open(bufname, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644)) >= 0) {
		hdr.h_magic = UMAGIC;
		hdr.h_nunits = nunits;
		hdr.h_ssize = send-sstart;
		if (write(bfd, &hdr, sizeof(hdr)) != sizeof(hdr))
			goto bad;
		if (write(bfd, sstart, hdr.h_ssize) != hdr.h_ssize)
			goto bad;
		for (i=0; i!=nunits; i++)
			{
			if (dflag) {
				fprintf (stderr, "u_name=%s u_val=units[i].u_val=%f\n",
					units[i].u_name, units[i].u_val);
				}
			units[i].u_name -= (long)sstart;/* Rel. address */
			}
		i = write(bfd, units, nunits*sizeof(UNIT));
    if (i == -1) goto bad;
		for (i=0; i!=nunits; i++)
			units[i].u_name += (long)sstart;
	bad:
		close(bfd);
	}
}

/*
 * Initialise by reading in units information
 * either in binary or ascii form and updating
 * the binary information.
 */
void init (void)
{
	if (!binary())
		update();
#ifdef MAINUNIT
	printf("%d units\n", nunits);
#endif
	peekc = EOF;
	if (uflag)
		exit(0);
}

void punit (UNIT *u)
{
	register int i;

	printf("%g", u->u_val);
	for (i=0; i!=NDIM; i++)
		if (u->u_dim[i] == 1)
			printf(" %s", units[i].u_name);
		else if (u->u_dim[i] > 0)
			printf(" %s+%d", units[i].u_name, u->u_dim[i]);
		else if (u->u_dim[i] < 0)
			printf(" %s-%d", units[i].u_name, -u->u_dim[i]);
	printf("\n");
}

int main (int argc, char *argv[])
{
	UNIT have, want;
	register int i;

	setbuf(stdout, NULL); /* no buffering for pipe use */
	setbuf(stderr, NULL);
	if (argc>1 && *argv[1]=='-') {
		if (argv[1][1]=='u' && argv[1][2]=='\0')
			uflag++;
		else if (argv[1][1]=='d' && argv[1][2]=='\0')
			dflag++;
		else {
			printf("Usage: mcunits [-u]\n");
			exit(1);
		}
	}
	init();
	setbuf(fd = stdin, inbuf);
Again:
	if (!getunit(&have, "You have: ")
	|| !getunit(&want, "You want: "))
		exit(0);
	for (i=0; i!=NDIM; i++)
		if (have.u_dim[i] != want.u_dim[i]) {
			printf("Conformability\n");
			punit(&have);
			punit(&want);
			goto Again;
		}
	printf("* %g\n/ %g\n", have.u_val/want.u_val, want.u_val/have.u_val);
	goto Again;
}
#else
void mcunitconvert(char *inbuf, char *havestr, char *wantstr) {
	UNIT have, want;
	register int i;

	if (!getunit(&have, havestr)
	 || !getunit(&want, wantstr)) {
			sprintf(inbuf, "Conformability\n");
  } else {
	for (i=0; i!=NDIM; i++)
		if (have.u_dim[i] != want.u_dim[i]) {
			sprintf(inbuf, "Conformability\n");
		} else {
      sprintf(inbuf, "* %g\n/ %g\n", have.u_val/want.u_val, want.u_val/have.u_val);
    }
  }
}
#endif
