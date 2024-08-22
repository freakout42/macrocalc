/* db2rdb.c
 * program to list the records in a dbase ii/iii/iv/dbman file
 * to the /rdb database-format
 * usage:	db2rdb [-opts] <filename
 * notes:	part of mc distribution
 */

#ifdef MSDOS
#include <io.h>
#include <fcntl.h>
#endif
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <arx_def.h>
#include <str_def.h>
#include <dbf_def.h>

void usage(void);

static void dblen (char *s, int *n, int *d)
{
char	*p;

*n	= atoi (s);
*d	= 0;
if ((p = strchr (s, '.')) != NULL) *d = atoi (p+1);
}

char		opts[] = "+ABF:KN:0a:bcdef:ghijklm:n:opqrs:tuvx:yz?";
char		tmpfname[81]	= "";

int main (int argc, char *argv[])
{
int		errornum;
int		i=0;
int		rec;
unsigned char	fld, fldrdb;
struct	DBF	*ddbf, *drdb;
struct FIELD_RECORD *f, *frdb;

extern int 	optind;
extern char 	*optarg;
int		c;
int		append	= FALSE;
int		external= FALSE;
char		*afile	= NULL;
char		*table	= NULL;
char		fieldse = '\t';
int		nullpa	= FALSE;
int		plus	= FALSE;
int		adsill	= FALSE;
int		adsfix	= FALSE;
int		cnvdate	= FALSE;
int		boolean	= FALSE;
int		erased	= FALSE;
int		recid	= FALSE;
int		fast	= FALSE;
int		dashlen	= FALSE;
int		dashlin	= TRUE;
int		dataproc= TRUE;
int		structure= FALSE;
int		array= FALSE;
int		setdate	= FALSE;
int		reverse	= FALSE;
int		mysql	= FALSE;
int		odbc	= FALSE;
int		pgsql	= FALSE;
int		yaml	= FALSE;
/*int		sqlite	= FALSE;*/
int		dotrim	= TRUE;
enum TODROP	todrop	= DROPIT;
enum TOWHAT	to_	= LEAVE;
int		verbose	= FALSE;
int		numlen	= 9;
int		numdec	= 2;
int		floatlen= 9;
int		floatdec= 2;
char		*filename= NULL;
int		year	= 0;
int		month	= 0;
int		day	= 0;
time_t		now;
struct tm	*tmdb;
struct file	*fi;
#ifdef LICENSED
int		license;

license = lib_akey ("000000", FALSE);
#endif
while ((c = getopt (argc, argv, opts)) != EOF)
	switch (c)
	 {
	 case '+':	plus	= TRUE;
			break;
	 case 'N':	dblen (optarg, &numlen, &numdec);
			break;
	 case 'F':	dblen (optarg, &floatlen, &floatdec);
			break;
	 case 'x':	external= TRUE;
			/*FALLTHRU*/
	 case 'a':	if (append) lib_fatl (USAGE,
				     "Append and External mode are exclusive");
			append	= TRUE;
			afile	= optarg;
			break;
	 case 'A':	adsill	= TRUE;
			break;
	 case 'B':	adsfix	= TRUE;
			break;
	 case '0':	nullpa	= TRUE;
			break;
	 case 'u':	to_	= UPPER;
			break;
	 case 'l':	to_	= LOWER;
			break;
	 case 'c':	to_	= CAPITALIZE;
			break;
	 case 'b':	boolean	= TRUE;
			break;
	 case 'd':	cnvdate	= TRUE;
			break;
	 case 'e':	erased	= TRUE;
			break;
	 case 'f':	fieldse	= *optarg;
			break;
	 case 'm':	table	= optarg;
			break;
	 case 'g':	dashlen	= TRUE;
			break;
	 case 'h':	dashlin	= FALSE;
			break;
	 case 'i':	dataproc= FALSE;
			break;
	 case 'j':	recid	= TRUE;
			break;
	 case 'k':	structure= TRUE;
			break;
	 case 'K':	array= TRUE;
			break;
	 case 'o':	todrop	= NODROP;
			break;
	 case 'p':	pgsql	= TRUE;
			break;
	 case 'y':	yaml	= TRUE;
			break;
	 case 'q':	mysql	= TRUE;
			break;
	 case 'z':	odbc	= TRUE;
							todrop	= NODROP;
			break;
	 case 'r':	reverse	= TRUE;
			to_	= UPPER;
			break;
	 case 's':	setdate	= TRUE;
			if (sscanf (optarg, "%2d%2d%2d", &year, &month, &day)
				!= 3) usage();
			if (year<=50) year += 100;
			break;
	 case 't':	dotrim	= FALSE;
			break;
	 case 'v':	verbose	= TRUE;
			break;
	 case '?':
	 default:	usage();
	 }
for (; optind < argc; optind++)
	{
	if (filename != NULL) usage();
	filename = argv[optind];
	}
ddbf = (struct DBF *) malloc (sizeof(struct DBF)); /* allocate space for DBF */
if (ddbf == NULL) lib_fatl (OUT_OF_MEM, "Cannot allocate memory");
drdb = (struct DBF *) malloc (sizeof(struct DBF)); /* allocate space for DBF */
if (drdb == NULL) lib_fatl (OUT_OF_MEM, "Cannot allocate memory");

if (reverse)
	{
	if (boolean) lib_mesg ("Boolean mode in reverse mode ignored");
	if (cnvdate) lib_mesg ("Date mode in reverse mode ignored");
	if (erased) lib_mesg ("Erased mode in reverse mode ignored");
	if (recid) lib_mesg ("RecordId mode in reverse mode ignored");
	if (mysql) lib_mesg ("MySQL mode in reverse mode ignored");
	if (odbc) lib_mesg ("ODBC mode in reverse mode ignored");
	if (pgsql) lib_mesg ("PgSql mode in reverse mode ignored");
	if (yaml) lib_mesg ("Yaml mode in reverse mode ignored");
	if (adsill||adsfix) lib_mesg ("Ads mode in reverse mode ignored");
#ifndef MSDOS
	if (filename != NULL && strcmp (filename, "-"))
		{
		strcpy (drdb->filename, filename);
		drdb->file_ptr = NULL;
		}
	else
		{
#endif
		strcpy (drdb->filename, "");
		drdb->file_ptr = stdin;
#ifdef MSDOS
/*		setmode (fileno(stdout), O_BINARY);
 */
		if (!structure && !array !(append && !external))
		   {
		   if (freopen (filename, "wb", stdout) == NULL)
			lib_fatl (NO_FILE, "Cannot open %s", filename);
		   }
#else
		}
#endif
	if (setdate)
		{
		drdb->p.update_yr	= year;
		drdb->p.update_mo	= month;
		drdb->p.update_day	= day;
		}
	else
		{
#ifndef MSDOS
		if (filename==NULL)
			{
#endif
			time (&now);
			tmdb = localtime (&now);
#ifndef MSDOS
			}
		else
			{
			if ((fi = lib_stat (filename)) == NULL)
				lib_fatl (NO_FILE, "Cannot stat %s", filename);
			tmdb = fi->time;
			}
#endif
		drdb->p.update_yr	= tmdb->tm_year;
		drdb->p.update_mo	= tmdb->tm_mon+1;
		drdb->p.update_day	= tmdb->tm_mday;
		}

	drdb->status	= fast ? not_updated : not_open;
	drdb->fieldsep	= fieldse;
	drdb->dashline	= dashlin;
	drdb->trim	= dotrim;
	drdb->todrop	= todrop;
	drdb->towhat	= to_;
	errornum = rdb_open(drdb);		/* open file	*/
	switch (errornum)
	 {
	 case OUT_OF_MEM:
		lib_fatl (errornum, "Not enough memory");
		/*FALLTHRU*/
	 case NO_TMP_FILE:
		lib_fatl (errornum, "Cannot open temporary file");
		/*FALLTHRU*/
	 case NO_FILE:
		lib_fatl (errornum, "Cannot open file");
		/*FALLTHRU*/
	 case BAD_FORMAT:
		lib_fatl (errornum,
			"File is not a /rdb database file");
	 }

	if (structure)
		{
		dbf_from (drdb, 0);
		exit(0);
		}

	if (array)
		{
		dbf_from (drdb, 1);
		exit(0);
		}

	if (append)
		{
		strcpy (ddbf->filename, afile);
		ddbf->file_ptr = NULL;
		errornum = dbf_open(ddbf);		/* open file	*/
		switch (errornum)
		 {
		 case OUT_OF_MEM:
			lib_fatl (errornum, "Not enough memory");
			/*FALLTHRU*/
		 case NO_FILE:
			lib_fatl (errornum, "Cannot open file %s", afile);
			/*FALLTHRU*/
		 case BAD_FORMAT:
			lib_fatl (errornum,
			 "File %s is not a dBASE II, III or IV file", afile);
		 }

		if (external)
			{
			fclose (ddbf->file_ptr);
			ddbf->p.records	= drdb->p.records;
			ddbf->file_ptr	= stdout;
			}
		else
			{
			i		=  ddbf->p.records;
			ddbf->p.records	+= drdb->p.records;
			}
		if (fseek (ddbf->file_ptr, 0L, SEEK_SET) != 0 || dbf_crea(ddbf) != 0)
			lib_fatl (IO_ERROR, "write error header");
		if (!external)
			fseek (ddbf->file_ptr, i*ddbf->p.record_length, SEEK_CUR);
		}
	else
		{
		*ddbf		= *drdb;
		ddbf->file_ptr	= stdout;
		}
	if (!append)
		{
		if (!plus) ddbf->p.header_length++;
		if (dbf_crea(ddbf) != 0)
			lib_fatl (IO_ERROR, "write error header");
		}
	else
		for (fldrdb=1, frdb = drdb->fields_ptr; fldrdb<=drdb->fields;
			fldrdb++, frdb++)
		{
		for (fld=1, f = ddbf->fields_ptr; fld<=ddbf->fields; fld++, f++)
			{
			if (!strncmp (frdb->name, f->name, MAX_FIELDNAME))
				{
#ifdef DEBUG
				fprintf (stderr, "exchg %d<->%d\n",fld,fldrdb);
#endif
				memcpy (frdb, f, sizeof (struct FIELD_RECORD));
				break;
				}
			}
		if (fld > ddbf->fields)
			{
			frdb->typ = 'X';
#ifdef DEBUG
			fprintf (stderr, "no_use %-11.11s\n", frdb->name);
#endif
			}
		}
	}
else
	{
	if (append) lib_fatl (USAGE, "No append mode in normal mode");
	if (plus) lib_mesg ("Plus mode in normal mode ignored");
	if (setdate) lib_mesg ("Setdate mode in normal mode ignored");
	if (nullpa) lib_mesg ("Nullpadding mode in normal mode ignored");
#ifndef MSDOS
	if (filename != NULL && strcmp (filename, "-"))
		{
#endif
		strcpy (ddbf->filename, filename);
		ddbf->file_ptr = NULL;
#ifndef MSDOS
		}
	else
		{
		strcpy (ddbf->filename, "");
		ddbf->file_ptr = stdin;
#ifdef MSDOS
/*		setmode (fileno(stdin), O_BINARY);
 */
#endif
		}
#endif

	errornum = dbf_open(ddbf);		/* open file	*/
	switch (errornum)
	 {
	 case OUT_OF_MEM:
		lib_fatl (errornum, "Not enough memory");
		/*FALLTHRU*/
	 case NO_FILE:
		lib_fatl (errornum, "Cannot open file");
		/*FALLTHRU*/
	 case BAD_FORMAT:
		lib_fatl (errornum, "File is not a dBASE II, III or IV file");
	 }
	if (structure)
		{
		dbf_from (ddbf, 0);
		exit(0);
		}
	if (array)
		{
		dbf_from (ddbf, 1);
		exit(0);
		}
	*drdb	= *ddbf;
	ddbf->todrop	= todrop;
	ddbf->towhat	= to_;
	}
if (verbose) dbf_info (ddbf);
for (fld=1, f = ddbf->fields_ptr; fld<=ddbf->fields; fld++, f++)
	{
	switch (f->typ)
	 {
	 case 'D':
	 case 'N':
	 case 'F':
	 case 'L':
	 case 'C':
		break;
	 case 'M':
		lib_mesg ("Memo field %.11s not supported", f->name);
		break;
	 default:
	 	lib_fatl (UNKNOWN_TYPE, "Unknown field type");
	 }
	}

    i = 0;
if (dataproc)
    {
	drdb->fieldsep	= fieldse;
	drdb->dashline	= dashlin;
	drdb->dashlength= dashlen;
	drdb->convdate	= cnvdate;
	drdb->trim	= dotrim;
	drdb->bool	= boolean;
	drdb->todrop	= todrop;
	drdb->towhat	= to_;

    if (reverse)
	{
	ddbf->nullpad	= nullpa;
	for (ddbf->current_record=1; rdb_read(drdb) != EOF; ddbf->current_record++)
		{
		rdb_2dbf (ddbf, drdb);
		dbf_writ (ddbf);
		i++;
		}
	}
    else if (mysql || odbc || pgsql || yaml)
	{
	drdb->ofile_ptr	= stdout;
	strcpy (drdb->tmpfilename, table==NULL?"":table);
	if (mysql) sql_crea (drdb);
	if (odbc) sql_crep (drdb);
	if (pgsql) sql_crep (drdb);
	if (yaml)  yml_crea (drdb);
	for (rec=1; rec<=ddbf->p.records; rec++)
		{
		if (dbf_read (ddbf)) lib_fatl (RECNO_TOO_BIG, "Cannot read record");
		if (*ddbf->record_ptr == '*' && !erased) continue;
		dbf_2rdb (drdb, ddbf);
		if (mysql) sql_writ (drdb);
		if (odbc) sql_wrio (drdb);
		if (pgsql) sql_wrip (drdb);
		if (yaml)  yml_writ (drdb);
		i++;
		}
	}
    else if (adsfix)
	{
	if (ddbf->towhat == CAPITALIZE) {
		fprintf(stderr, "USE %s EXCLUSIVE\n", table==NULL?"TABLE":table);
	}
	drdb->file_ptr	= stdout;
	if ((c = dbf_crea (drdb))) lib_fatl (IO_ERROR, "write error header %d", c);
        drdb->record_ptr = ddbf->record_ptr;
	for (rec=1; rec<=ddbf->p.records; rec++)
		{
		if (dbf_read (ddbf)) lib_fatl (RECNO_TOO_BIG, "Cannot read record");
		if (*ddbf->record_ptr == '*' && !erased) continue;
		c = dbf_afix (ddbf);
		dbf_writ (drdb);
		if (c) i++;
		}
	dbf_clos (drdb);
	if (ddbf->towhat == CAPITALIZE) {
		fprintf(stderr, "CLOSE DATA\n");
	}
	}
    else
	{
	drdb->ofile_ptr	= stdout;
	rdb_crea (drdb, recid);
	for (rec=1; rec<=ddbf->p.records; rec++)
		{
		if (dbf_read (ddbf)) lib_fatl (RECNO_TOO_BIG, "Cannot read record");
		if (*ddbf->record_ptr == '*' && !erased) continue;
		c = dbf_2rdb (drdb, ddbf);
		if (!adsill || c) {
			rdb_writ (drdb, recid);
			i++;
			}
		}
	}
    }
    rdb_clos (drdb);
    dbf_clos (ddbf);
return !(adsill||adsfix) ^ (i!=0);
}
