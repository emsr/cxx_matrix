//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

//
//      ostream_complex.cc      Add I/O routine for printing out complex
//                              numbers.
//

                                 
#include <complex.h>

// AT&T Cfront does not provide for cout << complex ...
//
ostream& operator<<(ostream &s, complex z)
{
    s << real(z) << " "  << imag(z) ;
    return s;
}

