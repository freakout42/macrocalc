/*
 * globargs.c
 * Link with getdir.asm to form a command line argument wildcard expander
 * for Turbo C 2.0.
 * Rename the real main() to _main().
 *
 * David MacKenzie
 * Latest revision: 05/08/88
 */

#include <stdio.h>
#include "dos.h"
#include "dir.h"

/* Arbitrary internal limit on filename path length. */
#define MAXPATHL 125

/* Granularity with which space for argv is allocated. */
#define ARG_INCR 10

char   *
malloc(), *realloc();

/*
 * Return a pointer to the last element of the given path.
 */
char   *
basename(p)
    char   *p;
{
    char   *tail;

    for (tail = p; *p; ++p)
	if (*p == ':' || *p == '\\')
	    tail = p + 1;
    return tail;
}

static void
overflow()
{
    fprintf(stderr, "Too many arguments.\n");
    exit(1);
}

static char **
initargv()
{
    char  **argv;

    argv = (char **) malloc(ARG_INCR * sizeof(char **));
    if (!argv)
	overflow();
    return argv;
}

/*
 * Return ptr to a copy of s in newly allocated memory.
 */
static char *
savestr(s)
    char   *s;
{
    char   *t;

    t = malloc(strlen(s) + 1);
    if (!t)
	overflow();
    (void) strcpy(t, s);
    return t;
}

/*
 * Save arg at position argc in array argv.
 */
static char **
savearg(arg, argv, argc, alloc)
    char   *arg;
    char  **argv;
    int     argc;
    int     alloc;		/* If true, allocate mem, otherwise copy ptr. */
{
    if (argc > 0 && argc % ARG_INCR == 0) {
	argv = (char **) realloc(argv, (argc + ARG_INCR) * sizeof(char **));
	if (!argv)
	    overflow();
    }
    argv[argc] = alloc ? savestr(arg) : arg;
    return argv;
}

int main(argc, argv, envp)
    int     argc;
    char  **argv;
    char  **envp;
{
    char    dirbuf[512];	/* Buffer for disk input. */
    struct ffblk *dir = (struct ffblk *) dirbuf;
    char    pathbuf[MAXPATHL];	/* Path of globbed arguments. */
    char   *tail;		/* Tail component of pathbuf. */
    int     nargc;
    char  **nargv;
    int     optind;

    setdta(dirbuf);		/* Set disk transfer area. */

    nargc = 0;
    nargv = initargv();
    for (optind = 0; optind < argc; ++optind) {
	if (!strchr(argv[optind], '*') && !strchr(argv[optind], '?'))
	    nargv = savearg(argv[optind], nargv, nargc++, 0);
	else if (!findfirst(argv[optind], getdta(), FA_DIREC)) {
	    /*
	     * Copy any leading drive and/or path into pathbuf because the
	     * MS-DOS globbing routines return only the base of the
	     * filenames. 
	     */
	    (void) strcpy(pathbuf, argv[optind]);
	    tail = basename(pathbuf);

	    do {
		if (dir->ff_name[0] != '.') {
		    (void) strcpy(tail, dir->ff_name);
		    nargv = savearg(pathbuf, nargv, nargc++, 1);
		}
	    } while (!findnext(getdta()));
	} else {
	    fprintf(stderr, "No match.\n");
	    exit(1);
	}
    }
    /* Some programs expect that argv[argc] is NULL. */
    nargv = savearg(NULL, nargv, nargc, 0);
    return _main(nargc, nargv, envp);
}
