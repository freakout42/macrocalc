.Id $Id: mclimit.r,v 1.4 1999/10/15 17:32:55 axel Exp $
.SH Limitations
.if \nV=4 \{\
Only 20 kBytes (approx) of cell info can be displayed in one worksheet.
This is due to the fact that
.B mc
reads a whole worksheet into memory and COHERENT 3.2
is limited to small model.
.LP
Special characters of other languages than english,
which are available through the keyboard-driver of COHERENT,
cannot be used with \fBmc\fP.
This is due to the fact that
.B mc
uses the COHERENT 3.2 curses library,
which is limited to a 7-bit character set.
.LP \}
The date and time functions do not work with dates prior to Dec 14, 1902.
The date display format does not work with dates prior to Jan 1, 1970
and after Jan 18, 2038 due to UNIX's date limits.
.LP
Since there is a bug in cursor positioning of the COHERENT curses-library,
the lower-right position of the screen is never displayed.
.LP
The function shortcut for SUM(Range) to write only Range (mc V<=3.01)
was removed from mc V4 due to parser problems with range names.
