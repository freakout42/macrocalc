.Id $Id: mcmath.r,v 1.2 1997/06/23 04:17:27 axel Exp $
.SH "Mathematical functions"
The  following functions act upon the argument 'x' within
the parentheses. 'x' can be a value or a formula
which references other cells within the worksheet.
The (C) indicates availability for complex values.
.IP ABS(x) 3
Returns the absolute value of argument.
.br
ex. ABS(-10) = 10
.IP ACOS(x) 3
Arc Cosine, returns radians. (C)
.br
ex. ACOS(0.5) = 1.0471
.IP ASIN(x) 3
Arc Sine, returns radians. (C)
.br
ex. ASIN(0.866) = 1.0471
.IP ATAN(x) 3
Arc Tangent, returns radians. (C)
.br
ex. ATAN(1.732) = 1.047
.IP COS(x) 3
Cosine of angle, x in radians. (C)
.br
ex. COS(1.047) = 0.5
.IP EXP(x) 3
Raises 'e' to the 'x'-th power. (C)
.br
ex. EXP(5) = 148.4131591
.IP INT(x) 3
Integer value of argument. The fractional
part of the number is truncated  and  the
integer remainder is returned.
.br
ex. INT(10.6) = 10
.IP LN(x) 3
Natural Logarithm to base 'e'. (C)
.br
ex. LN(6) = 1.791...
.IP LOG(x) 3
Logarithm to base '10'. (C)
.br
ex. LOG(10) = 1
.IP SIN(x) 3
Sine of angle, x in radians. (C)
.br
ex. SIN(1.047) = 0.866
.IP SQRT(x) 3
Square root of argument. (C)
.br
ex. SQRT(2) = 1.4142135624
.IP TAN(x) 3
Tangent of angle, x in radians. (C)
.br
ex. TAN(1.047) = 1.732
