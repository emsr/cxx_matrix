//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

#ifndef _MV_BLAS1_float_H_
#define _MV_BLAS1_float_H_

#include <math.h>
#include <stdlib.h>


MV_Vector_float& operator*=(MV_Vector_float &x, const float &a);
MV_Vector_float operator*(const float &a, const MV_Vector_float &x);
MV_Vector_float operator*(const MV_Vector_float &x, const float &a);
MV_Vector_float operator+(const MV_Vector_float &x, 
    const MV_Vector_float &y);
MV_Vector_float operator-(const MV_Vector_float &x, 
    const MV_Vector_float &y);
MV_Vector_float& operator+=(MV_Vector_float &x, const MV_Vector_float &y);
MV_Vector_float& operator-=(MV_Vector_float &x, const MV_Vector_float &y);

float dot(const MV_Vector_float &x, const MV_Vector_float &y);
float norm(const MV_Vector_float &x);

#endif
// _MV_BLAS1_float_H_
