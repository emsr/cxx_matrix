//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//


// this is really used as a sort of global constant. The reason
// for creating its own type is that so it can be overloaded to perform
// a deep or shallow assignement.  (Any variable of type MV_Matrix_::ref_type
// has only one possible value: one.)

#ifndef _MV_MATRIX_REF_
#define _MV_MATRIX_REF_
struct MV_Matrix_
{
    enum ref_type  { ref = 1};
} ;
# endif
