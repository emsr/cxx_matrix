//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

#ifndef _MV_BLAS1_double_H_
#define _MV_BLAS1_double_H_

#include <math.h>
#include <stdlib.h>


MV_Vector_double& operator*=(MV_Vector_double &x, const double &a);
MV_Vector_double operator*(const double &a, const MV_Vector_double &x);
MV_Vector_double operator*(const MV_Vector_double &x, const double &a);
MV_Vector_double operator+(const MV_Vector_double &x, 
    const MV_Vector_double &y);
MV_Vector_double operator-(const MV_Vector_double &x, 
    const MV_Vector_double &y);
MV_Vector_double& operator+=(MV_Vector_double &x, const MV_Vector_double &y);
MV_Vector_double& operator-=(MV_Vector_double &x, const MV_Vector_double &y);

double dot(const MV_Vector_double &x, const MV_Vector_double &y);
double norm(const MV_Vector_double &x);

#endif
// _MV_BLAS1_double_H_
