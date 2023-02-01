.Id mcformu.r
.SH Formulas
Formulas are entered like
.Hr -symbolic Text text.
.Hr text.
.B mc
will determine whether the entered string is a legal formula or
.Hr -symbolic Text text.
.Hr text.
If a string starts like a formula,
.B mc
assumes a syntax error and put the cursor in the place where the error starts.
If the cell is text and not a formula, simply press
.I [Escape]
or begin the text with one of the
.Hr -symbolic Justification "justification signs."
.Hr "justification signs."
.LP
Cell names in formulas are typed in with the column followed by the row enclosed
in square brackets separated by semicolon.
Relative cell references start with a sign.
An absolute cell reference can be written without brackets or comma.
Toggle absolute cell references in formulas with relative references by pressing F4 (or $).
Named Ranges can be used directly with the rangename.
.DS
ex. A1+[A;2]*[B;-1]^5/([+0;+2]+SUM(Myrange))
.DE
.P
.BP SPECIAL
Formulas can be used recursively!
That means that in spite of copying a formula cell,
the absolute cell reference preceded by ampersand (\fB&\fP) can be used.
This saves memory and, when changing the formula,
all formulas entered in this way
will change automatically, because they refer to the same
formula text string!
Think of this as a way of defining user functions in a worksheet.
.DS
ex. A1: value:   1.00
    B1: formula: 2*[-1;+0] = 2.00
    A2: value:   2.00
    B2: formula: &B1       = 4.00
.DE
.P
.BP CONSTANTS
\fBmc\fP recognizes the following special constant values in formulas:
.ST
.IP PI 8
Value of PI = 3.141592.
.IP E 8
Value of E = 2.718281.
.IP I 8
Value of I = 0i1 the imaginary unit.
.IP SQRT2 8
Value of sqrt(2) = 1.414213.
.IP COL 8
Current column number.
.IP ROW 8
Current row number.
.ET
.P
.Ha Strings ""
.BP STRINGS
Strings in formulas are entered enclosed in 
quotation marks (\fB"\fP).
If a cell is preceded by hash (\fB#\fP) it refers to the cell string
instead of the value.
Strings can be concatenated without any operator.
