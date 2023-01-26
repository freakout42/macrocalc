/* mcomplex.c 2023/01/24 axel */

#include <math.h>
#include <complex.h>
#include "mcelldef.h"
#include "mcfunc.h"

#ifndef complex
#define complex _Complex
#endif

struct CELLVALUE cpxmult(struct CELLVALUE f1, struct CELLVALUE f2) {
double complex z1, z2, p1;
struct CELLVALUE r1;

z1 = f1.value + f1.cimag * I;
z2 = f2.value + f2.cimag * I;
p1 = z1 * z2;
r1.value = creal(p1);
r1.cimag = cimag(p1);
r1.unit = (void*)0;

return r1;
}

struct CELLVALUE cpxdivd(struct CELLVALUE f1, struct CELLVALUE f2) {
double complex z1, z2, p1;
struct CELLVALUE r1;

z1 = f1.value + f1.cimag * I;
z2 = f2.value + f2.cimag * I;
p1 = z1 / z2;
r1.value = creal(p1);
r1.cimag = cimag(p1);
r1.unit = (void*)0;

return r1;
}

struct CELLVALUE cpxpow(struct CELLVALUE f1, struct CELLVALUE f2) {
double complex z1, z2, p1;
struct CELLVALUE r1;

z1 = f1.value + f1.cimag * I;
z2 = f2.value + f2.cimag * I;

#if defined(__FreeBSD__)
p1 = HUGE_VAL + 0.0 * I;
#else
p1 = cpow(z1, z2);
#endif

r1.value = creal(p1);
r1.cimag = cimag(p1);
r1.unit = (void*)0;

return r1;
}

struct CELLVALUE cpxfunc1(int fcode, struct CELLVALUE f1) {
double complex z1, z2, z3, z4, p1;
struct CELLVALUE r1;

z1 = f1.value + f1.cimag * I;
switch(fcode) {
 case L3_ABS:
  p1 = cabs(z1); break;
 case L3_ACOS:
  p1 = cacos(z1); break;
 case L3_ASIN:
  p1 = casin(z1); break;
 case L3_ATAN:
  p1 = catan(z1); break;
 case L3_COS:
  p1 = ccos(z1); break;
 case L3_COSH:
  p1 = ccosh(z1); break;
 case L3_EXP:
  p1 = cexp(z1); break;
#if !defined(__FreeBSD__)
 case L3_LN:
  p1 = clog(z1); break;
 case L3_LOG:
  z4 = 10.0 + 0.0 * I;
  z3 = clog(z4);
  z2 = clog(z1);
  p1 = z2 / z3;
  break;
#endif
 case L3_SIN:
  p1 = csin(z1); break;
 case L3_SINH:
  p1 = csinh(z1); break;
 case L3_SQRT:
  p1 = csqrt(z1); break;
 case L3_TAN:
  p1 = ctan(z1); break;
 case L3_TANH:
  p1 = ctanh(z1); break;
 default:
  p1 = HUGE_VAL + 0.0 * I; break;
 }
r1.value = creal(p1);
r1.cimag = cimag(p1);
r1.unit = (void*)0;

return r1;
}

