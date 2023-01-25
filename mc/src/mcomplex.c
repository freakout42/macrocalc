/* mcomplex.c 2023/01/24 axel */

#include <complex.h>
#include "mcelldef.h"

struct CELLVALUE cpxmult(struct CELLVALUE f1, struct CELLVALUE f2) {
double complex z1;
double complex z2;
double complex p1;
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
double complex z1;
double complex z2;
double complex p1;
struct CELLVALUE r1;

z1 = f1.value + f1.cimag * I;
z2 = f2.value + f2.cimag * I;
p1 = z1 / z2;
r1.value = creal(p1);
r1.cimag = cimag(p1);
r1.unit = (void*)0;

return r1;
}

struct CELLVALUE cpxsqrt(struct CELLVALUE f1) {
double complex z1;
double complex p1;
struct CELLVALUE r1;

z1 = f1.value + f1.cimag * I;
p1 = csqrt(z1);
r1.value = creal(p1);
r1.cimag = cimag(p1);
r1.unit = (void*)0;

return r1;
}

