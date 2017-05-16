//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

#include <iostream.h>
#include <stdlib.h>

#include "mv_matrix.h"

void foo(MV_ColMat<double> *Aptr)
{
    MV_ColMat<double> &A = *Aptr;

    A = 0.0;
}

main(int argc, char *argv[])
{
    if (argc<7)
    {
        cout << "Usage " << argv[0] << " M N start0 end0  start1 end1" << endl;
        exit(1);
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int s0 = atoi(argv[3]);
    int e0 = atoi(argv[4]);
    int s1 = atoi(argv[5]);
    int e1 = atoi(argv[6]);

    cout << "Using M = " << M << endl;
    cout << "Using N = " << N << endl;
    cout << "Using start0 = " << s0 << endl;
    cout << "Using end0   = " << e0 << endl;
    cout << "Using start1 = " << s1 << endl;
    cout << "Using end1   = " << e1 << endl << endl;;


    MV_ColMat<double> A(M,N);
    MV_ColMat<double> B(M,N);

    A = 3.0;
    B = 7.0;

    A( M/2, N/2) = 0.0;
    cout << " A=3.0;   A(M/2, N/2) = 0.0 " << endl;
    cout << A << endl;


    MV_VecIndex I(s0, e0);
    MV_VecIndex J(s1, e1);


    cout << "I = (" << s0 << ":"<< e0 <<") " << endl;
    cout << "J = (" << s1 << ":"<< e1 <<") " << endl;

    A(I,J+1) = B(I,J);
    cout << "A(I,J+1) = B(I,J) " <<  endl << A << endl;


    A(I,J) = 8.0;
    cout << "A(I,J) = 8.0 " <<  endl << A << endl;

#if 0
// The following test section attempts to take the address of a temporary
// result and hence not compilable on all C++ platforms.  

    cout << "calling foo(&A(I)) " << endl;
    foo(&A(I));
    cout << "A(I)" << endl << A << endl;

    MV_ColMat<int> C(M, N);
    MV_ColMat<int> &c = C(I,J);

    C = 1;
    c = 0;

    cout << "After renaming c to be C(I,J), and executing c = 0.0 "
        << endl << C << endl;
#endif

    return (0);
}

