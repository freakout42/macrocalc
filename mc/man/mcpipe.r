.Id $Id: mcpipe.r,v 1.8 1997/06/28 15:25:45 axel Exp $
.SH "Piping"
.B mc
has a very advanced interface to the operating system via piping.
This offers unmatched flexibility based
upon the Unix operating system's piping mechanism.
Since the database
.B /rdb
is based upon the shell level this can be used perfectly for database
interactions like queries or updates in tables.
.LP
A cell statement with the syntax:
.DS
\fIcell\fP<\fIcommand\fP
.DE
.P
is called a cell-command.
When such a cell is entered,
it will execute the command and put the command's output into
the range of cells starting with cell as the upper-left corner.
The output is read line by line into the rows of the range.
The columns, which have to be separated by
.B <tab>
in the output of the command,
are placed into the columns of the range.
At the end of the data a special cell value designated 'EOF' (end of file)
is placed in the cell below the data.
Therefore,
.B /rdb
commands work especially well with
.B mc
because this is exactly the format of 
.B /rdb
tables.
But also all other standard Unix commands like
.B "sed, awk, cut, ..."
can produce output of this format.
It is also simple to produce input-files with any standard editor,
or write own programs which can feed \fBmc\fP.
Since formulas can be used recursively with the '&' operator;
you can also build flexible commands of querying or updating a database.
.DS
ex. B2<cat expenses
.DE
.P
Puts the table
.I expenses
into the cell range B2..F5.
.DS
ex. B2<row 'Onhand < &F15' <inventory
.DE
.P
Select all rows in the
.I inventory
table in which
.I Onhand
is less than the value in the cell F15
into the cell range B2..G5.
.LP
A cell statement of the form:
.DS
\fIcell\fP<<\fIcommand\fP
.DE
.P
will act the same way but does not overwrite the data in the output range.
The data is inserted into the worksheet
by inserting a new row into the worksheet for every line in the commands output.
Existing cell contents in the first row are copied into every inserted row.
This is useful, when a formula should act on every line of the output.
.LP
A cell statement of the form:
.DS
\fIcell\fP<!\fIcommand\fP
.DE
.P
will act the same way but parses the incoming cells as formulas.
This is especially useful with the \fBmcread\fP command.
This command is part of the system
and can extract the formulas
of a given range or the whole worksheet
from a \fBmc\fP-file from standard input.
It inserts
.B <tab>
between the formulas of the selected columns.
With this combination any
.Hr -symbolic Range range
.Hr range
from a extern worksheet-file
can be easily copied to a range in the current worksheet.
For example the command:
.DS
f9<!mcread -rb3-d7 <somedata.mc
.DE
.P
selects all constants/formulas from range B3.D7 in the worksheet-file
.I somedata.mc
and copies them to the range starting with cell F9
into the current worksheet.
.P
This procedure has the advantage that the source file and range
are stored in the command-cell,
and can always be re-imported by re-editing and
.Hr -symbolic Reexecute "re-executing the command."
.Hr "re-executing the command (see below)."
The disadvantage is that the attributes and formats are not imported,
but once they are set up,
they are used by the next import and keep intact.
.P
Call
.B mcread
with the
.I -h
option to get a reference of the command.
.LP
A cell statement of the form:
.DS
\fIcell\fP>\fIcommand\fP
.DE
.P
will execute the command and feed the cell range starting with cell
into the input of the command.
The cell range starts with cell in the left upper corner until it
reaches an empty cell right of the starting cell,
and until it reaches the 'EOF'-cell on the way down the worksheet.
.LP
The cell statement:
.DS
B2>cat >expenses
.DE
.P
places the updated table from the worksheet back into the database.
To determine where to stop, the special 'EOF'-cell must be found.
It can be entered by placing the string "EOF" into the cell
below the data, as long as it was not written by a former query.
.LP
.Ha Reexecute ""
To reexecute any cell-command simply reedit it with
.I "[Edit] [Return]."
.BP "ERROR MESSAGES"
All output to standard error of cell-commands is redirected to the
.B mc
error message line.
