/* $Id: dbf_def.h,v 1.14 2021/12/16 12:02:35 axel Exp $
 * purpose:	header file defining structures
 * 		and error codes for dbase access routines.
 * notes:	this file must be included in any programs
 * 		which use the dbase access routines included
 * 		do not use -a option with this header file.
 */

#define	DBVERSION	0x03
#define	DBMANFILE	1
#define	DB2FILE		2
#define	DB3FILE		3
#define	DB4FILE		4
#define	DB4MEMO		0x04
#define	DB4SQL		0x38
#define	DB3PLUS		0x40
#define	DB3MEMO		0x80
#define	DBMAN		11245
#define	DBMARK		'\r'
#define DBMAN_HDLG	256
#define	FIELD_REC_LEN	32u	/* length of field description record	*/
#define	HEADER_PROLOG	32u	/* length of header without field desc and terminator */
#define	MAX_HEADER	4129	/* maximum length of dBase III header	*/
#define	MAX_RECORD	4000	/* dBase III record limit		*/
#define	MAX_FIELDNAME	11	/* dBase III field name length		*/
#define	MAX_FIELDS2	32	/* dBase II field limit			*/
#define	MAX_FIELDS	128	/* dBase III field limit		*/
#define	MAX_FIELDSIZE	256	/* dBase III field size limit		*/
#define	MAX_NFIELD	15	/* dBase III float field		*/
#define	MAX_FFIELD	30	/* dBase III float field		*/
#define	MAX_FFIELD	30	/* dBase III float field		*/
#define	N_RESERVED	14	/* dBase III reserved bytes		*/

/* /rdb usage of reserved bytes	*/
#define maxlen		reserved_bytes[0]/* maxlen		*/

/* error codes */
#define	USAGE		1	/* command sytax error		*/
#define	OUT_OF_MEM	8	/* insufficient memory error	*/
#define	NO_FILE		2	/* file not found error		*/
#define	NO_TMP_FILE	3	/* tmp file not found error	*/
#define	IO_ERROR	4	/* file i/o-error		*/
#define	BAD_FORMAT	11	/* file not dBASE III file	*/
#define	UNKNOWN_TYPE	12	/* unknown field type		*/
#define	RECNO_TOO_BIG	105	/* requested record too big	*/

#define	TAB '\t'

typedef unsigned char UCHAR;

#include <noalign.h>
PACKED struct FIELD_RECORD	/* This structure is filled in memory	*/
	{			/* with a fread.	do not change.	*/
	char	name[MAX_FIELDNAME] PACK2;/* name of field in asciz	*/
	UCHAR	typ PACK2;	/* type of field...char,numeric etc.	*/
	int	field_data_address PACK2;/* offset of field in record	*/
	UCHAR	len PACK2;	/* length of field			*/
	UCHAR	dec PACK2;	/* decimals in field			*/
	UCHAR	reserved_bytes[N_RESERVED] PACK2;/* reserved by dbase	*/
	};

PACKED struct FIELD_RECORD2	/* This structure is filled in memory	*/
	{			/* with a fread.	do not change.	*/
	char	name[MAX_FIELDNAME] PACK2;/* name of field in asciz	*/
	UCHAR	typ PACK2;	/* type of field...char,numeric etc.	*/
	UCHAR	len PACK2;	/* length of field			*/
	short	field_data_address PACK2;/* offset of field in record	*/
	UCHAR	dec PACK2;	/* decimals in field			*/
};

PACKED struct FIELD_DBMAN	/* This structure is filled in memory	*/
	{			/* with a fread.	do not change.	*/
	char	name[MAX_FIELDNAME] PACK2;/* name of field in asciz	*/
	UCHAR	typ PACK2;	/* type of field...char,numeric etc.	*/
	short	len PACK2;	/* length of field			*/
	UCHAR	dec PACK2;	/* decimals in field			*/
	UCHAR	rdprctn PACK2;	/* read protection			*/
	UCHAR	wrprctn PACK2;	/* read protection			*/
	UCHAR	attrib PACK2;	/* attributes				*/
	UCHAR	reserved_bytes[10] PACK2;/* reserved by dbman		*/
	};

PACKED struct DBF
{
char	filename[MAXPATH] PACK2;/* filename				*/
char	tmpfilename[MAXPATH] PACK2;/* filename for temp file		*/
FILE	*file_ptr PACK2;	/* c file pointer			*/
FILE	*ofile_ptr PACK2;	/* c file pointer			*/
unsigned int current_record PACK2;/* current record in memory		*/
enum			 	/* status of file			*/
	{
	not_open=0,
	not_updated,
	updated
	} status PACK2;
char	fieldsep PACK2;		/* field separator			*/
char	dashline PACK2;		/* dashline				*/
char	dashlength PACK2;	/* dashlength				*/
char	convdate PACK2;		/* convert dates			*/
char	trim PACK2;		/* trim strings				*/
char	bool PACK2;		/* boolean				*/
char	nullpad PACK2;		/* nullpadding				*/
enum TODROP {DROPIT, NODROP}
	todrop PACK2;
enum TOWHAT {LEAVE, UPPER, LOWER, CAPITALIZE}
	towhat PACK2;
UCHAR	fields PACK2;		/* number of fields			*/
struct FIELD_RECORD *fields_ptr PACK2;/* pointer to field array		*/
char	*record_ptr PACK2;	/* pointer to current record struct	*/

/* the following struct is filled with a fread,
 * do not change order or size */
PACKED struct DBFPROLOG
	{
	UCHAR	dbf_version PACK2;/* version character			*/
	UCHAR	update_yr PACK2;/* date of last update - year (-1900)	*/
	UCHAR	update_mo PACK2;/* date of last update - month		*/
	UCHAR	update_day PACK2;/* date of last update - day		*/
	unsigned int records PACK2;/* number of records in dbf		*/
	unsigned short	header_length PACK2;/* length of header structure*/
	unsigned short	record_length PACK2;/* length of a record 	*/
	short	reserved PACK2;
	UCHAR	transaction PACK2;	/* not committed */
	UCHAR	crypted PACK2;
	UCHAR	net[12] PACK2;		/* network */
	UCHAR	keymark PACK2;
	UCHAR	dummy[3] PACK2;
	} p;
};

/* the following structis filled with a fread,
 * do not change order or size */
PACKED struct DBFPROLOG2
	{
	UCHAR	dbf_version PACK2;/* version character			*/
	unsigned short records PACK2;/* number of records in dbf	*/
	UCHAR	update_yr PACK2;/* date of last update - year (-1900)	*/
	UCHAR	update_mo PACK2;/* date of last update - month		*/
	UCHAR	update_day PACK2;/* date of last update - day		*/
	unsigned short record_length PACK2;/* length of a record 	*/
	};

/* the following structis filled with a fread,
 * do not change order or size */
PACKED struct DBMANPROLOG
	{
	union	{
		unsigned short	dbman_version;	/* version character	*/
		UCHAR		dbman_ver[2];	/* version character2	*/
		} v PACK2;
	unsigned short	dbman_filetype PACK2;	/* file type		*/
	unsigned short	dbman_filever PACK2;	/* file version		*/
	UCHAR		update_yr PACK2;/* date of last update - year (-1900)*/
	UCHAR		update_mo PACK2;/* date of last update - month	*/
	UCHAR		update_day PACK2;/* date of last update - day	*/
	UCHAR		update_filler PACK2;/* date of last update - day*/
	unsigned short	dbman_res[17] PACK2;/* reserved			*/
	unsigned int	records PACK2;	/* number of records in dbman	*/
	unsigned short	record_length PACK2;/* length of a record 	*/
	unsigned short	ddesc_length PACK2;/* length of data descriptor	*/
	unsigned short	header_length PACK2;/* length of header structure*/
	char		filler[202] PACK2;/* filler			*/
	unsigned short	field_count PACK2;/* fields in database		*/
	};
#include <align.h>

int rdb_open (struct DBF *d);
int rdb_crea (struct DBF *d);
int rdb_clos (struct DBF *d);
int rdb_read (struct DBF *d);
int rdb_writ (struct DBF *d);
int rdb_2dbf (struct DBF *td, struct DBF *sd);
int sql_crea (struct DBF *d);
int sql_crep (struct DBF *d);
int sql_writ (struct DBF *d);
int sql_wrip (struct DBF *d);
int sql_wrio (struct DBF *d);
int yml_crea (struct DBF *d);
int yml_writ (struct DBF *d);
int dbf_open (struct DBF *d);
int dbf_clos (struct DBF *d);
int dbf_crea (struct DBF *d);
int dbf_read (struct DBF *d);
int dbf_writ (struct DBF *d);
int dbf_2rdb (struct DBF *td, struct DBF *sd);
int dbf_afix (struct DBF *td);
void dbf_from (struct DBF *d);
void dbf_info (struct DBF *d);
void oem858_to_iso885915(char *output);
void iso885915_to_oem858(char *output);
