.Id $Id: mcfiles.r,v 1.11 1997/06/28 15:07:53 axel Exp $
.SH Files
The installation of
.B mc
creates the following files:
.ST
.IP $ARX/bin/mc 6
the spreadsheet program.
.IP $ARX/bin/mcunits 6
the unit conversion program,
which is used by the
.Hr -symbolic Option_u "\fI-u\fP option"
.Hr "\fI-u\fP option"
of \fBmc\fP.
.IP $ARX/bin/mcunits.rdb 6
the unit conversion programs units table
(see \fBmcunits\fP for more information).
.IP $ARX/bin/mcrdb 6
script for editing \fB/rdb\fP-tables with \fBmc\fP.
.IP $ARX/bin/mc2conv 6
script for converting from \fBmc\fP V1.x to V2.x
(compatible with all higher versions).
.IP $ARX/bin/mc2wks 6
file conversion program from and to Lotus WKS-file-format.
.IP $ARX/bin/mcread 6
\fBmc\fP file reader for copying ranges from external files
into the current worksheet via the cell-command "<!".
.IP $ARX/lib/mcsample.tar 6
example worksheets to demonstrate the power of \fBmc\fP.
.IP $ARX/lib/mcrdb.mc 6
worksheet for script \fBmcrdb\fP.
.IP $ARX/lib/mcmacro 6
program for reading macros.
.IP $ARX/lib/mcprint 6
script for printing.
.IP $ARX/lib/mcellval 6
script for linking.
.IP $ARX/lib/mcmsg 6
program for reading message file.
.IP $ARX/lib/mcmsg.rdb 6
Message file.
.IP $ARX/lib/mcmenu.rdb 6
Menu file.
.IP $ARX/mc/src/* 6
development library files, see
.Hr -symbolic "User functions" "User functions."
.Hr "User functions."
.IP $ARX/bin/arxactiv 6
program to add the serial number and the license key to the binary.
.ET
