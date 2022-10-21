/* $Id: lotus.h,v 1.13 2006/09/27 08:06:26 axel Exp $
 * definitions for lotus file format
 */

#ifndef LOTUS
#define LOTUS
#endif

#define	BUFL		512	/* max bufferlength for lotus record	*/
#define	T_WKS		1028	/* ident of lotus v1.x file		*/
#define	T_WKR		1029	/* ident for synphony file		*/
#define	T_WK1		1030	/* ident for lotus v2.x file		*/
				/* opcodes				*/
#define	L_BOF		0x00	/* opcode for begin of file		*/
#define	LL_bof		2
#define	L_EOF		0x01
#define	LL_eof		0
#define	L_CALCMODE	0x02
#define	LL_calcmode	1
#define	L_CALCORDER	0x03
#define	LL_calcorder	1
#define	L_SPLIT		0x04
#define	LL_split	1
#define	L_SYNC		0x05
#define	LL_sync		1
#define	L_WINDOW1	0x07
#define	LL_window1	31
#define	L_COLW1		0x08
#define	LL_colw1	3
#define	L_NAME		0x0b
#define	LL_name		24
#define	L_INTEGER	0x0d
#define	LL_integer	7
#define	L_NUMBER	0x0e
#define	LL_number	13
#define	L_LABEL		0x0f
#define	LL_label	245
#define	L_FORMULA	0x10
#define	LL_formula	2063
#define	L_PROTEC	0x24
#define	LL_protec	1

#define F_CONSTANT	0x00
#define F_VARIABLE	0x01
#define F_RANGE		0x02
#define F_RETURN	0x03
#define F_PARANTHESES	0x04
#define F_INTEGER	0x05
#define F_STRING	0x06
#define F_UNARYMINUS	0x08
#define F_ADDITION	0x09
#define F_SUBSTRACTION	0x0a
#define F_MULTIPLY	0x0b
#define F_DIVISION	0x0c
#define F_EXPONENT	0x0d
#define F_EQUALS	0x0e
#define F_NOTEQUALS	0x0f
#define F_LESSEQUAL	0x10
#define F_GREATEREQUAL	0x11
#define F_LESS		0x12
#define F_GREATER	0x13
#define F_AND		0x14
#define F_OR		0x15
#define F_NOT		0x16
#define F_UNARYPLUS	0x17
#define F_STRINGCAT	0x18
#define F_UNIT		0x1a

#include "noalign.h"
typedef struct PACKED {char v[8];} mydouble;

typedef struct PACKED
	{
	short col PACK;
	short row PACK;
	} CELLADR123;

typedef	struct PACKED	/* lotus record structure		*/
	{
	short	opcode;	/* opcode record type			*/
	short	reclen;	/* record data length in bytes		*/
	union	{
		struct	{
			short		ver PACK;
			} bof PACK2;
		struct	{
			char		splitype PACK2;
			} split PACK2;
		struct	{
			char		synctype PACK2;
			} sync PACK2;
		struct	{
			short		curcol PACK;
			short		currow PACK;
			unsigned char	format PACK2;
			char		unused0 PACK2;
			short		width PACK;
			short		numcols PACK;
			short		numrows PACK;
			short		leftcol PACK;
			short		windowrow PACK;
			short		topcols PACK;
			short		toprows PACK;
			short		leftcol2 PACK;
			short		toprow PACK;
			short		borderwidthcol PACK;
			short		borderwidthrow PACK;
			char		unused1 PACK2;
			short		windowwidth PACK;
			} window1;
		struct	{
			short		col PACK;
			char		width PACK2;
			} colw1;
		struct	{
			char		rangename[16] PACK2;
			CELLADR123	cell1 PACK2;
			CELLADR123	cell2 PACK2;
			} name;
		struct	{
			unsigned char	format PACK2;
			CELLADR123	cell PACK2;
			short		value PACK;
			} integer;
		struct	{
			unsigned char	format PACK2;
			CELLADR123	cell PACK2;
			mydouble	value PACK2;
			} number;
		struct	{
			unsigned char	format PACK2;
			CELLADR123	cell PACK2;
			char		s[240] PACK2;
			} label;
		struct {
			unsigned char	format PACK2;
			CELLADR123	cell PACK2;
			mydouble	value PACK2;
			short		size PACK;
			unsigned char	code[2048] PACK2;
			} formula;
		struct	{
			unsigned char	protect PACK2;
			} protec;
		} data;
	} Lotrec;
#include "align.h"
