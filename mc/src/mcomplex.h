/* mcomplex.h 2023/01/24 axel */

struct CELLVALUE cpxmult(struct CELLVALUE f1, struct CELLVALUE f2);
struct CELLVALUE cpxdivd(struct CELLVALUE f1, struct CELLVALUE f2);
struct CELLVALUE cpxfunc1(int fcode, struct CELLVALUE f1);
struct CELLVALUE cpxpow(struct CELLVALUE f1, struct CELLVALUE f2);
