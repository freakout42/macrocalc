.Id mcmac.r
.SH Macros
A macro is a sequence of keystrokes or commands which can
be  performed automatically, just like a program within the spreadsheet.
Unlike most spreadsheets,
macros in \fBmc\fP are stored in files instead of inside the worksheet.
A macro can be executed with the [\fIMacro\fP]-key.
The macro will continue to execute until end-of-file.
An auto-execute macro (see
.Hr -symbolic Option_o "option \fI-o\fP"
.Hr "option \fI-o\fP"
) starts executing
immediately after the worksheet is loaded.
.LP
Every letter in the macro is simply fed into
.B mc
as normal input.
Exceptions are
.B <tab>
and \fB<newline>\fP;
they do nothing.
All macro keys
.\"and substitute function keys
must be enclosed in curly braces {}.
An optional repeat count can be appended.
.BP "MACRO KEYS"
When encountered in a macro file while executing,
they make \fBmc\fP behave as if the
.Hr -symbolic "Function keys" "corresponding key"
.Hr "corresponding key"
was pressed.
.DS
{LEFT}   - cursor left    {BIGLEFT}  - one page left
{RIGHT}  - cursor right   {BIGRIGHT} - one page right
{UP}     - cursor up      {PGUP}     - one page up
{DOWN}   - cursor down    {PGDN}     - one page down

{HOME}   - cursor to A1
{EDIT}   - F2             {GOTO}     - F5
{CALC}   - F9             ~          - Sub for [ENTER]
{ESC}    - Escape         {END}      - End Key
{INS}    - Insert key     {DEL}      - Del key
{MENU}   - Call main menu {MARK}     - Mark cell
.DE
.P
.BP ENVIRONMENT
Variables from the process-environment can be included in the macro
by preceding them with "$".
.BP COMMENTS
Lines starting with "#" are completely ignored and are therefore comments.
.LP
Try the following simple macro which enters the string
"hello, world" into the current cell and
moves the cursor down one cell and right two cells.
Type the following in file "test.mcr" with an editor of your choice:
.DS
# this is a comment
hello, world~
{DOWN}{RIGHT 2}
.DE
.P
and call the macro with [\fIMacro\fP] and the filename "test.mcr".
.BP LIMITATIONS
There are no higher macro statements implemented yet.
