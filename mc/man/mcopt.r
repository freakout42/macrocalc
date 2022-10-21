.Id $Id: mcopt.r,v 1.14 2002/10/17 12:25:47 axel Exp $
.SH Options
.Ha Option_l ""
.IP \\fI-l\\fP
Sets alternative screen layout with input line at the bottom.
.Ha Option_b ""
.IP \\fI-b\\fP
Suppress displaying the borders with column and row numbering.
.Ha Option_s ""
.IP \\fI-s\\fP
Suppress displaying current cell in line 1 of the screen.
This is especially interesting on slow terminals,
since moving around is much faster.
.Ha Option_m ""
.IP \\fI-m\\fP
Suppress displaying the menu, marked range and the recalculation message
for slow terminal lines.
.Ha Option_h ""
.IP \\fI-h\\fP
Suppress highlighting of the marked range for slow terminal lines.
.Ha Option_c ""
.IP \\fI-c\\fP\\fBw\\fP
Sets preset column width to \fBw\fP. Default is 11.
.Ha Option_d ""
.IP \\fI-d\\fP\\fBp\\fP
Sets preset decimal places to \fBp\fP. Default is 2.
.Ha Option_f ""
.IP \\fI-f\\fP\\fBF\\fP
Sets preset format to \fBF\fP. Default is General.
Possible formats are:
.ST
.IP \\fBF\\fP
Fixed
.IP \\fBS\\fP
Science
.IP \\fBC\\fP
Currency
.IP \\fB,\\fP
Comma
.IP \\fBP\\fP
Percent
.IP \\fBD\\fP
Date
.IP \\fBT\\fP
Time
.IP \\fBG\\fP
General
.ET
.Ha Option_y ""
.IP \\fI-y\\fP\\fBc\\fP
Sets currency symbol to \fBc\fP. Default is dollar (\fB$\fP).
.Ha Option_a ""
.IP \\fI-a\\fP
No autocalc mode.
.Ha Option_r ""
.IP \\fI-r\\fP
Read-only mode.
Worksheets cannot be saved.
This is useful, when output is done by "cell>command" statements.
See shell-script
.Hr -symbolic mcrdb \fBmcrdb\fP
.Hr \fBmcrdb\fP 
for an example.
.Ha Option_o ""
.IP \\fI-o\\fP\\fBmacrofile\fP
Auto-execute macro found in \fBmacrofile\fP after loading the worksheet.
This option also suppresses the interactive startup message.
The \fBmacrofile\fP "-" only suppresses the interactive startup message.
.Ha Option_n ""
.IP \\fI-n\\fP\\fBmacrocommands\fP
Auto-execute macros in \fBmacrocommands\fP after loading the worksheet.
This option also suppresses the interactive startup message.
.Ha Option_x ""
.IP \\fI-x\\fP
Sets command autoexec mode.
All
.Hr -symbolic Piping cell-commands
.Hr cell-commands
"cell<command" found in a new loaded worksheet and
"cell>command" found in a saved worksheet
will be executed automatically.
.Ha Option_u ""
.IP \\fI-u\\fP
Starts
.B $ARX/mcunits
as a background process for
.Hr -symbolic Units "unit conversion"
.Hr "unit conversion"
calculation.
The "units" function of
.B mc
can only work with this option!
Startup is slower with this option,
so only use it if unit conversion is needed.
.Ha Option_p ""
.IP \\fI-p\\fP
Pipes the standard input (a worksheet file) into the standard
output (\fB[nt]roff\fP source format) without interactive action.
This \fBmust\fP be the last option when present!
This is especially interesting because
.B mc
uses a fully transparent, editable
.Hr -symbolic "File format" "ASCII file format,"
.Hr "ASCII file format,"
which is compatible with the
.B /rdb
database system.
The worksheet file can easily be edited or manipulated with almost any UNIX
command such as
.B sed
or \fBawk\fP.
You can also use \fB/rdb\fP's
100-plus powerful database commands.
.Ha Option_q ""
.IP \\fI-q\\fP
Pipes the standard input (a worksheet file) into a
"comma-separated-values" file
without interactive action.
This \fBmust\fP be the last option when present!
.Ha Option_v ""
.IP \\fI-v\\fP
Prints version information.
