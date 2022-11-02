# macrocalc
mc - MacroCALC is a Lotus 1-2-3 compatible character based spreadsheet that
supports 100000 rows, 700 columns, 40 functions, display formats, file
linking, macro programming and user definable functions. MacroCALC is fully
integrated with [nt]roff, units, man, awk, perl and sh.

As it should be in the UNIX-environment, mc can act as a filter enabling users
to use pipes to perform complex transforms on streams of data. It contains a
C programming interface as well as Lotus WKS and dBASE file support. mc's own
file-format is flat ASCII, easy to understand and can therefore be directly
manipulated with awk, perl or other standard UNIX-tools, or even with editors
like vi.

MacroCALC is the only spreadsheet that manages units: 1 mile + 1 km = 1.62
miles; 1 km / 1 hour * 30 sec = 8.33 m. This can be seen as a physics engine.

It's git-maintained, lint-clean ANSI-C source is based on yacc, lex and
[n]curses. It runs on every terminal or emulation that has a correct
terminfo-entry. The user-interface is very convenient and well-known by 1-2-3
users. Function- and cursor-keys and extended video attributes are supported.

The menus and messages are in separate editable files and can be easily
translated into any language. Every command in the package has a detailed
man-page.

mc was a professional commercial product, sold a couple hundred times for AIX,
UnixWare, SCO, Coherent and Interactive during the 1990-2005 years - now going
open-source under the "Simplified BSD License".
