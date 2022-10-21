.Id $Id: mcoper.r,v 1.4 1997/06/25 19:18:21 axel Exp $
.SH Operators
An operator is a function that  is built into
.B mc
formula language.
It
usually  relates  one  operand  to  another.
The following is a table of the
.B mc
operators:
.DS
Op  Description
--  -----------------------------
^   Power
*   Multiplication
/   Division
+   Addition
-   Subtraction
==  Equivalence
!=  Inequality
<   Less than
<=  Less than or equal to
>   Greater than
>=  Greater than or equal to
&&  Logical AND
||  Logical OR
.DE
.LP
In addition to these C-like operators,
also operators known from other commonly used spreadsheet programs (e.g.:
#AND#, #OR#, ...) can be used.
.SH Precedence
Precedence refers to  the order in  which
.B mc
executes operators.
Cell formula languages assign a level of  precedence to each operator.
Operators are  executed in the  order of their  precedence level,
from highest to lowest.
.LP
The  following table  summarizes the  precedence of
.B mc
operators.
They are listed in descending order of precedence:  those listed
higher in the table are executed before those lower in the table.
Operators  listed  on  the  same  line  have the  same  level  of
precedence, and the  implementation determines the order in which
they are executed.
If you use  two or more such operators in the
same expression, you would be wise to use parentheses to indicate
exactly the order in which you want the operators executed.
.DS
Operator      Precedence
------------  -------------
(  )          Left to right
-             Right to left
^             Right to left
*  /          Left to right
+  -          Left to right
<  <=  >  >=  Left to right
==  !=        Left to right
&&            Left to right
||            Left to right
.DE
.LP
.B mc
always determines precedence in an expression by enclosing
sub-expressions  within   parentheses:  the  expression  enclosed
within the innermost parentheses is always executed first.
.BP SIMPLY
All of the rules of basic algebra apply to expressions in \fBmc\fP.
Remember to use parentheses to indicate exactly the order in which you want
the operators executed.
