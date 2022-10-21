.Id $Id: mclang.r,v 1.5 1997/06/25 19:18:21 axel Exp $
.SH "Language support"
.B mc
is language configurable.
The error messages and the menus are held in separate editable files,
which are read in at startup of \fBmc\fP.
The files can be translated into any language and can be altered
by the "bin" user with any editor.
.LP
The messages are held in the Message file
\fI$ARX/lib/mcmsg.rdb\fP.
This file is a \fB/rdb\fP-table (see
.Hr -symbolic "File format" "File format"
.Hr "File format"
).
To alter the file simply reedit only the messages without altering
the rest of the file.
.LP
The menus are in the Menu file
\fI$ARX/lib/mcmnu.rdb\fP.
This is a simple list of the available menus.
To alter the file simply reedit every menu keyword without altering
the order of the keywords!
The "hot-key" for every menu item is the first uppercase or
non-alphabetic character in every keyword.
.BP PLEASE
If you translate the files to any language,
please send the translated files to me via Email or on a disk.
They will then be distributed with \fBmc\fP.
