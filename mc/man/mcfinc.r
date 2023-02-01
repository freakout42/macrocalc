.Id $Id: mcfinc.r,v 1.3 1997/06/26 18:46:28 axel Exp $
.SH "Financial functions"
.IP CTERM(INTerest;FutureValue;PresentValue) 3
The number of compounding periods required for an
investment of PV (present value) to grow to FV (future
value), if the investment is earning INT interest per
compounding period.
.IP DDB(Cost;Salvage;Life;Period) 3
Accelerated depreciation value for an asset, given the
initial cost, the life expectancy, the end value and the
depreciation period.  The depreciation is calculated
using the double declining balance method.
.IP "FV(Annuity;interest rate;period)" 3
Returns the future value corresponding to  an  annuity
occurring over the specified period.  Reverse of the
above.
.br
ex. FV(700;0.0925;30) = 99979.77
.IP PMT(Principal;interest;period) 3
Returns the amount of an annuity equal to the
principal amortized over the period.
.br
ex. PMT(75000;0.0925;30) = 7462.61
.IP "PV(Annuity;interest rate;period)" 3
Returns the present value corresponding  to  an  annuity
occurring over the specified period.  Reverse of the
above.
.br
ex. PV(700;0.0925;30) = 7035.08
.IP RATE(FutureValue;PresentValue;N) 3
Interest rate required for an investment of PVAL to be
worth FVAL, in N compounding periods.
.IP SLN(Cost;Salvage;Life) 3
Straight line depreciation of an asset over ONE period
of LIFE.
.IP SYD(Cost;Salvage;Life;Period) 3
Depreciation of an asset using the Sum-of-Years
depreciation method.
.IP TERM(Payment;Interest;FutureValue)
Number of periods required to accumulate an investment
of FVAL making regular payments of PMT and accruing
interest at the rate of INT per period.
