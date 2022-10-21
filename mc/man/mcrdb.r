.Id $Id: mcrdb.r,v 1.3 1997/06/28 15:07:53 axel Exp $
.SH mcrdb
The shell-script \fBmcrdb\fP uses
.B mc
as a table editor for structured text-files or \fB/rdb\fP-tables.
With the command:
.DS
mcrdb \fItable\fP
.DE
.P
the file (or \fB/rdb\fP-table) "table" is locked and loaded into a worksheet.
.B mc
is called for editing the table in a much more convenient way than with
\fBve\fP,
the \fB/rdb\fP table-editor.
The user can see more than one row simultaneously,
and all spreadsheet-power can be used to modify the data.
Inserting, copying and deleting rows or fields is very easy.
It is a good idea to examine the shell-script how it works as an
example of using the advanced integration possibilities of
.B mc
and \fB/rdb\fP.
Of course
.B mcrdb
can only work when
.B /rdb
is installed.
