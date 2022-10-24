/*
 * getdir.h
 *
 * Definitions helpful for use with getdir.asm (find first & next match).
 * Names are borrowed from Power C.
 */

/* File attribute values. */
#define FA_NORMAL	0x00
#define FA_RDONLY	0x01
#define FA_HIDDEN	0x02
#define FA_SYSTEM	0x04
#define FA_LABEL	0x08
#define FA_DIREC	0x10
#define FA_ARCH		0x20
#define FA_ALL		0x16

/* The directory entry format as read from the disk. */
struct ffblk {
    char    ff_reserved[21];
    char    ff_attrib;
    /* format of time: hour:5, minute:6, twosec:5 */
    unsigned ff_ftime;
    /* format of date: year:7, month:4,  day:5 */
    unsigned ff_fdate;
    unsigned long ff_fsize;
    char    ff_fname[13];
    char    ff_fill[85];
};
