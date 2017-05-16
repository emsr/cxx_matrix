//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

#ifndef _MV_BLAS1_TYPE_H_
#define _MV_BLAS1_TYPE_H_

#include <math.h>
#include <stdlib.h>


MV_Vector_TYPE& operator*=(MV_Vector_TYPE &x, const TYPE &a);
MV_Vector_TYPE operator*(const TYPE &a, const MV_Vector_TYPE &x);
MV_Vector_TYPE operator*(const MV_Vector_TYPE &x, const TYPE &a);
MV_Vector_TYPE operator+(const MV_Vector_TYPE &x, 
    const MV_Vector_TYPE &y);
MV_Vector_TYPE operator-(const MV_Vector_TYPE &x, 
    const MV_Vector_TYPE &y);
MV_Vector_TYPE& operator+=(MV_Vector_TYPE &x, const MV_Vector_TYPE &y);
MV_Vector_TYPE& operator-=(MV_Vector_TYPE &x, const MV_Vector_TYPE &y);

TYPE dot(const MV_Vector_TYPE &x, const MV_Vector_TYPE &y);
TYPE norm(const MV_Vector_TYPE &x);

#endif
// _MV_BLAS1_TYPE_H_
