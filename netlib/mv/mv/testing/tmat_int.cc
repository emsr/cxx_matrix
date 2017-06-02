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
#include MATRIX_H

// #include "systime.c"

// example function to zero-out a matrix:  illustrates how to pass
// matrixes by "reference" (modifiable), as opposed to by "value"
//
void foo(MATRIX_int *Aptr)
{
    MATRIX_int &A = *Aptr;

    A = (int) 0;
}

// example funciton to return MV_ColMats:  (returns matrix + scalar)
//
MATRIX_int fexample(const MATRIX_int A, int s)
{
    int M = A.size(0);
    int N = A.size(1);
    MATRIX_int B(M,N);

    for (int j=0; j<N; j++)
        for (int i=0; i<M; i++)
            B(i,j) = A(i,j) + s;

    return B;
}


main(int argc, char *argv[])
{
    if (argc<7)
    {
        cout << "Usage: " << " M N Istart Iend Jstart Jend" << endl;
        exit(1);
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int Istart = atoi(argv[3]);
    int Jstart = atoi(argv[4]);
    int Iend = atoi(argv[5]);
    int Jend = atoi(argv[6]);
    int i,j;
    cout << "Using M = " << M << " N = " << N << 
            "   Istart = " << Istart << "   Iend = " <<  Iend <<
            "   Jstart = " << Jstart << "   Jend = " <<  Jend << endl;



    MATRIX_int C;
    cout << " Test null constructor:  MV_ColMat C() " << endl;
    cout << C << endl;


    MATRIX_int A(M,N);
    cout << " Test MV_ColMat(int,int) constructor:  MV_ColMat A(M,N) " << endl;
    cout << " values should be uninitalized.. " << endl;
    cout << A << endl;

    MATRIX_int B(M,N, 3);
    cout << " Test MV_ColMat(int, int, val) constructor:  MV_ColMat B(N, 3.0) " 
                << endl;
    cout << "   all values should be 3:  " << endl;
    cout << B << endl;

    cout << " Test A(i,j) indexing, set A(i,j) = 100*j + i, and B(i) = -A(i,j)"
        << endl;

    for (j=0; j<N; j++)
        for (i=0; i<M; i++)
        {
            A(i,j) = 100*i+j;
            B(i,j) =  - A(i,j);
        }

    cout << " A " << endl;
    cout << A << endl;
    cout << " B " << endl;
    cout <<  B << endl;



    cout << "Testing MV_VecIndex I(Istart, Iend) " << endl;
    //
    MV_VecIndex I(Istart, Iend);
    MV_VecIndex J(Jstart, Jend);
    //
    cout << "I = (" << Istart << ":"<< Iend <<") " << endl;
    cout << "J = (" << Jstart << ":"<< Jend <<") " << endl;

    cout << "Test A(I,J) = B(I,J) " <<  endl ;
    A(I,J) = B(I,J);
    cout << A << endl;

    cout << "Testing A(I,J) = 1.1 " << endl;
    A(I,J) = 11;
    cout << "A" << endl << A << endl;

    cout << "Testing MV_VecIndex + operators: " << endl;
    cout << " A = 0.0; A(I+1,J-1) = 1.0; " << endl;
    //
    A = (int) 0; 
    A(I+1,J-1) = (int) 1;
    //
    cout << " A " << endl << A << endl;

#if 0
    MV_VecIndex K( N/2, N/2 );
    cout << "Testing MV_VecIndex +/- operators: " << endl;
    cout << " J = MV_VecIndex( N/2, N/2 );" << endl;
    cout << " A = 0.0; A(J+1) = B(J-1); " << endl;
    //
    A = 0.0; 
    A(J+1) = B(J-1);
    //
    cout << " A " << endl << A << endl;

    cout << "Test = when matrices are overlapping  " << endl;
    cout << " A(MV_VecIndex(0,N-1)) = B; A(J+1) = A(J-1); " << endl;
    //
    A(MV_VecIndex(0, N-1)) = B; 
    A(J+1) = A(J-1);
    //
    cout << " A " << endl << A << endl;


    cout << "Testing MATRIX_int &c = C(I)" << endl;
    MATRIX_int &c = C(I);
    cout << " c " << endl;
    cout << c << endl;

    cout << "Testing c = B(I) " << endl;
    c  = B(I);
    cout << "c " << endl;
    cout << c << endl;

    cout << " C " << endl;
    cout << C << endl;

    cout << "Testing passing submatrixs to functions: calling foo(&c) " 
        << endl << "  where foo zeros out a matrix " << endl;
    foo(&c);
    cout << "C" << endl << C << endl;

    MATRIX_int F(N, 90);
    cout << endl << "Test constructors from exisitng memory : " << endl;
    cout <<  " MATRIX_int E(&F(0),N) " << endl;

    MATRIX_int E(&F[0], N);
    cout << "E : " << endl << E << endl;

    cout << " Now modify F(0) = 999 " << endl;
    F(0) = (int) 999;
    cout << "E should not be changed:" << endl;
    cout << E << endl;

    cout << "Test reference constructors from exisitng memory : " << endl;
    cout <<  " MATRIX_int G(&F(0),N, MV_Matrix_::ref) " << endl;

    MATRIX_int G(&F[0], N, MV_Matrix_::ref);

    cout << " Now modify F(0) = 999 " << endl;
    F(0) = (int) 999;
    cout << "G " << endl;
    cout << G << endl;

    cout << "Now set G = 5, F should change also " << endl;
    //
    G = (int) 5;
    //
    cout << " F: " << endl << F << endl;


    cout << "Testing returning submatrixs from functions:  " 
        << endl << "   MATRIX_int K = fexample(A,1); " << endl;
    //
    MATRIX_int K = fexample(A,1);
    //
    cout << "A" << endl << A << endl;
    cout << "K" << endl << K << endl;
#endif


    return 0;
}
