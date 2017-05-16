//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

#ifndef _MV_BLAS1_int_H_
#define _MV_BLAS1_int_H_

#include <math.h>
#include <stdlib.h>


MV_Vector_int& operator*=(MV_Vector_int &x, const int &a);
MV_Vector_int operator*(const int &a, const MV_Vector_int &x);
MV_Vector_int operator*(const MV_Vector_int &x, const int &a);
MV_Vector_int operator+(const MV_Vector_int &x, 
    const MV_Vector_int &y);
MV_Vector_int operator-(const MV_Vector_int &x, 
    const MV_Vector_int &y);
MV_Vector_int& operator+=(MV_Vector_int &x, const MV_Vector_int &y);
MV_Vector_int& operator-=(MV_Vector_int &x, const MV_Vector_int &y);

int dot(const MV_Vector_int &x, const MV_Vector_int &y);
int norm(const MV_Vector_int &x);

#endif
// _MV_BLAS1_int_H_
