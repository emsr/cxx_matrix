//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

//
//      tmat.cc        Testing routine for MV++ matrix class
//
//      Usage:  <M> <N> <block-index start>  <block-index end>
//
//      (Suggest these to be relatively small positive numbers, as  
//      output is proportional to "length".)
//
//      This test program is self-explanatory (there are diagnostics
//      output message of what each step is performing)and excercise 
//      most of the MV_ColMat methods, thus providing examples of how 
//      these are used.
//


#include <iostream.h>
#include <stdlib.h>

#include "vector_defs.h"
#include VECTOR_H


int
main(int argc, char *argv[])
{
    if (argc<2)
    {
        cout << "Usage: " << " M " << endl;
        exit(1);
    }

    int N = atoi(argv[1]);
    int j;
    cout << "Using N = " << N << " N = " << N << endl;

    VECTOR_double x(N), y(N), z(N);

    for (j=0; j<N; j++)
    {
        x(j) = j;
        y(j) = j / 100;
    }

    cout << " x " << endl;
    cout << x << endl;

    cout << "Testing Vector * scalar:   " << endl;
    z = x * 2;
    cout << " z = x * 2 : " << endl;
    cout << z << endl;
    z = z * 2;
    cout << " z = z * 2 : " << endl;
    cout << z << endl;


    cout << "Testing Vector *= scalar:  " << endl;
     (x *= 2) *= 2;
    cout << " (x *= 2) *= 2: " << endl;
    cout << x << endl;

    cout << "Testing Vector += Vector:  " << endl;
    x = (double) 2; 
    y = (double) 1;

    cout << "x=2, y=1; x += y: " << endl;
    x += y;
    cout << x << endl;

    cout << "Testing Vector -= Vector:  " << endl;
    x = (double) 2; 
    y = (double) 1;

    cout << "x=2, y=1; x -= y: " << endl;
    x -= y;
    cout << x << endl;


    return 0;
}
