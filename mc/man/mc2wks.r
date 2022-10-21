.Id $Id: mc2wks.r,v 1.9 1997/06/28 15:07:53 axel Exp $
.SH "Lotus WKS-files"
A conversion program for the most popular DOS spreadsheet file format WKS
(Lotus 1-2-3) is included in \fBmc\fP.
.B mc2wks
converts
\fBmc\fP-files
to and from binary WKS-files.
With the command
.DS
mc2wks [-\fIoption\fP] <\fImcfile\fP >\fIwksfile\fP
.DE
.P
every worksheet produced with
.B mc
can be converted to a WKS-file.
.B mc2wks
can handle also the \fBmc\fP-options \fI\-c \-d \-f \-v\fP and \fI\-x\fP
the same way
.B mc
does.
.ig
.P
Additionally the \fI\-7\fP and \fI\-8\fP-options
force 7 or 8-bit translation of
8-bit characters in both directions.
The translation is done between the
.I "Lotus International Character Set"
and the
.I "IBM 8-bit Code Page 437"
and vice versa.
The IBM set is used by COHERENT (and most other UNIX consoles).
Default is
.ie \nV=4 7-bit
.el 8-bit
mode.
.if \nV=4 \{\
The 7-bit translation should be used,
because this version of
.B mc
cannot display and handle 8-bit characters (see Limitations). \}
.P
With the COHERENT
.B dos
command those WKS-files can be copied to a MS-DOS filesystem or floppy disk,
and then loaded into any DOS spreadsheet for further processing.
Most DOS spreadsheets support the Lotus WKS-file-format.
.BP NOTE
Do not use the
.B a
ASCII option of the
\fBdos\fP-command
when transferring the WKS-files!
WKS-files are
.I no
transparent ASCII-files but are binary!
..
.LP
With the command
.DS
mc2wks -r <\fIwksfile\fP >\fImcfile\fP
.DE
.P
every worksheet produced with
a DOS spreadsheet which has been transferred to UNIX
(or is on a UNIX-DOS shared device)
can be converted to a \fBmc\fP-file.
.ig
.BP TIP
The COHERENT
.B dos
command (Version >=3.2) has a
.I pipe
option.
With the commands (they can be given on one line)
.DS
mc2wks </u/rick/car.mc |
dos rp /dev/dos /usr/mike/car.wk1
.DE
.P
and
.DS
dos xp /dev/dos /usr/mike/car.wk1 |
mc2wks -r >/u/rick/car.mc
.DE
.P
copying to and from dos and converting to and from mc-file-format can be done
in one command and without intermediate files!
Another advantage is the independence of the directory structures on DOS
and COHERENT.
In this example the DOS file:
.DS
C:\\USR\\MIKE\\CAR.WK1
.DE
.P
is converted to and from the COHERENT file
.DS
/u/rick/car.mc
.DE
.if \nV>4 \{\
.P
On COHERENT 4 and on UNIX System V the commands
.B doscat
and
.B doscp
do similar things,
but with a different and more dos-like syntax. \}
..
.P
.BP LIMITATIONS
.B mc2wks
can only convert Lotus-WKS- and -WK1-files produced by
Lotus Versions 1-2-3 V<=2.x.
The newer WK3-files must be converted to WK1-format by the
1-2-3 utility-program
.B TRANS.EXE
before using
.B mc2wks
with them.
.ST
.IP units 16
Since no DOS spreadsheet can handle units, all unit information is lost
when converting from
.B mc
to WKS.
.IP "range names" 16
Range names longer than 16 characters will be shortened to 16 chars
when transferred to WKS.
Since range names in
.B mc
are case sensitive and in DOS spreadsheets not,
they are converted to uppercase
when transferred to WKS.
.IP "user functions" 16
User functions and functions special for
.B mc
(i.e. @LINK()) will probably generate error messages when loaded with the DOS
spreadsheet, but conversion is done.
.IP functions 16
Cells containing functions of DOS spreadsheets
that are not implemented in
.B mc
can not be transferred to mc-file-format.
The rest of the worksheet is transferred and an error message is displayed.
.\"Remove calls to those functions with the DOS spreadsheet from the worksheet
.\"before transferring.
.IP arguments
Multiple arguments of statistical functions are not implemented in \fBmc\fP.
Therefore only the first argument is recognized when converting to
mc-file-format.
.ET
.LP
.B "Lotus 1-2-3"
is a trademark of Lotus Development Corp.
