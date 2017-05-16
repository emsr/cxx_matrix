//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

#ifndef _MV_BLAS1_complex_H_
#define _MV_BLAS1_complex_H_

#include <math.h>
#include <stdlib.h>


MV_Vector_complex& operator*=(MV_Vector_complex &x, const complex &a);
MV_Vector_complex operator*(const complex &a, const MV_Vector_complex &x);
MV_Vector_complex operator*(const MV_Vector_complex &x, const complex &a);
MV_Vector_complex operator+(const MV_Vector_complex &x, 
    const MV_Vector_complex &y);
MV_Vector_complex operator-(const MV_Vector_complex &x, 
    const MV_Vector_complex &y);
MV_Vector_complex& operator+=(MV_Vector_complex &x, const MV_Vector_complex &y);
MV_Vector_complex& operator-=(MV_Vector_complex &x, const MV_Vector_complex &y);

complex dot(const MV_Vector_complex &x, const MV_Vector_complex &y);
complex norm(const MV_Vector_complex &x);

#endif
// _MV_BLAS1_complex_H_
