.Id $Id: mcstat.r,v 1.2 1997/06/23 04:17:27 axel Exp $
.SH "Statistical functions"
Statistical  functions  operate  on  a  range  of  values.
A range specifies a rectangular block of data in the worksheet.
Only ONE range may be used as an argument.
.br
ex. Range = A1..B10 defines the block of data from:
.DS
      a1...b1
       .   .
       .   .
       .   .
     a10...b10
.DE
.P
Presented  below  are  the  statistical  functions
available  within mc.
To illustrate the use  of
each  function  an  example  is included.
All  examples will operate on the numbers contained in the
portion of the worksheet duplicated below, A1..C3.
.DS
              A        B        C
        1       1.1      5.6      8.2
        2       2.3      7.5       10
        3       3.5      4.4        2
        4
.DE
.P
.IP AVG(Range) 3
Average value of the range.
.br
ex. AVG(A1..C3) = 4.955555
.IP COUNT(Range) 3
Count the number of elements in the range.  Blank cells ignored.
.br
ex. COUNT(A1..C3) = 9; COUNT(A1..A4) = 3
.IP MAX(Range) 3
Maximum value in the range.
.br
ex. MAX(A1..C3) = 10
.IP MIN(Range) 3
Minimum value in the range.
.br
ex. MIN(A1..C3) = 1.1
.IP STD(Range) 3
Standard deviation of the range.
.br
ex. STD(A1..C3) =  2.904827
.IP SUM(Range) 3
Sum of values in the range.
.br
ex. SUM(A1..C3) = 44.6
.br
or   A1..C3 = 44.6
.IP VAR(Range) 3
Returns the population variance of the range.
.br
ex. VAR(A1..C3) = 8.438024
