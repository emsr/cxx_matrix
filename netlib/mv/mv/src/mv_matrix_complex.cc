//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

//
//      mv_matrix_complex.cc      Basic vector class (complex precision)
//

#include "mv_matrix_complex.h"

unsigned int MV_ColMat_complex::size(int i) const 
{
    if (i==0) return dim0_;
    if (i==1) return dim1_;
    else
    {
     cerr << "Called MV_ColMat::size(" << i << ")  must be 0 or 1 " << endl;
     exit(1);
    }

    // never should be here, but many compilers warn about not
    // returning a value
    return 0;
}

// NOTE: null construct have ref_ flag turned OFF, otherwise, we can
//          never reset the size of matrix....
MV_ColMat_complex::MV_ColMat_complex()  : v_(), dim0_(0), dim1_(0) , lda_(0), ref_(0){}
                                                                

MV_ColMat_complex::MV_ColMat_complex(unsigned int m, unsigned int n) : v_(m*n),
        dim0_(m), dim1_(n), lda_(m), ref_(0) {}

MV_ColMat_complex::MV_ColMat_complex(unsigned int m, unsigned int n, const complex &s) : v_(m*n),
        dim0_(m), dim1_(n), lda_(m), ref_(0) 
{
    operator=(s);
}

// operators and member functions






MV_ColMat_complex& MV_ColMat_complex::operator=(const complex & s) 
{
    int M = size(0);
    int N = size(1);

    if (lda_ == M)      // if continuous, then just assign as a ?
        v_ =  s;        // single long vector.

    else
    {
    // this should run much faster than the just accessing each (i,j)
    // element individually 
    //

        MV_VecIndex I(0,M-1);
        for (int j=0; j<N; j++)
        {
            v_(I) = s;
            I += lda_;
        }
    }

    return *this;
}

MV_ColMat_complex& MV_ColMat_complex::newsize(unsigned int M, unsigned int N)
{
    v_.newsize(M*N);
    dim0_ = M;
    dim1_ = N;
    lda_ = M;

    return *this;
}

MV_ColMat_complex& MV_ColMat_complex::operator=(const MV_ColMat_complex & m) 
{

    int lM = dim0_;     // left hand arg  (this)
    int lN = dim1_;

    int rM = m.dim0_;   // right hand arg (m)
    int rN = m.dim1_;


    // if the left-hand side is a matrix reference, the we copy the
    // elements of m *into* the region specfied by the reference.
    // i.e. inject().

    if (ref_)
    {
        // check conformance,       
        if (lM != rM  || lN != rN)      
        {
            cerr << "MV_ColMatRef::operator=  non-conformant assignment.\n";
            exit(1);
        }
    }
    else
    {
        newsize(rM,rN);
    }

    // at this point the left hand and right hand sides are conformant

    // this should run much faster than the just accessing each (i,j)
    // element individually 

    // if both sides are contigous, then just copy as one vector
    if ( lM == lda_ && rM == m.lda_)
    {
        MV_VecIndex I(0,rM*rN-1);
        v_(I) = m.v_(I);
    }
    else
    {
        // slower way...

        MV_VecIndex I(0,rM-1);
        MV_VecIndex K(0,rM-1);
        for (int j=0; j<rN; j++)
        {
            v_(I) = m.v_(K);
            I += lda_;
            K += m.lda_;
        }
    }

    return *this;   
}

MV_ColMat_complex::MV_ColMat_complex(const MV_ColMat_complex & m) : 
        v_(m.dim0_*m.dim1_), dim0_(m.dim0_),
        dim1_(m.dim1_), ref_(0), lda_(m.dim0_)
{

    int M = m.dim0_;
    int N = m.dim1_;

    // this should run much faster than the just accessing each (i,j)
    // element individually 

    MV_VecIndex I(0,M-1);
    MV_VecIndex K(0,M-1);
    for (int j=0; j<N; j++)
    {
        v_(I) = m.v_(K);
        I += lda_;
        K += m.lda_;
    }
}



MV_ColMat_complex::MV_ColMat_complex(complex* d, unsigned int m, unsigned int n) :
    v_(m*n), dim0_(m), dim1_(n), lda_(m), ref_(0)
{
    int mn = m*n;

    // d is contiguous, so just copy 1-d vector
    for (int i=0; i< mn; i++)
            v_[i] = d[i];
}


MV_ColMat_complex::MV_ColMat_complex(complex* d, unsigned int m, unsigned int n, 
        unsigned int lda) :
    v_(m*n), dim0_(m), dim1_(n), lda_(lda), ref_(0)
{
    for (int j=0; j< n; j++)
        for (int i=0; i<m; i++)
            operator()(i,j) = d[j*lda + i];   // could be made faster!!
}


MV_ColMat_complex MV_ColMat_complex::operator()(const MV_VecIndex &I, const MV_VecIndex &J)
{
    // check that index is not out of bounds
    //
    if (I.end() >= dim0_  || J.end() >= dim1_)
    {
        cerr << "Matrix index: (" << I.start() << ":" << I.end()  
             << "," << J.start() << ":" << J.end()   
             << ") not a subset of (0:" << dim0_ - 1 << ", 0:" 
             << dim1_-1 << ") " << endl;
        exit(1);
    }

    // this automatically returns a reference
    // 
    return MV_ColMat_complex(&v_[J.start()*lda_ + I.start()], 
            I.end() - I.start() + 1, 
            J.end() - J.start() + 1, lda_, MV_Matrix_::ref);
}

const MV_ColMat_complex MV_ColMat_complex::operator()(const MV_VecIndex &I, 
    const MV_VecIndex &J) const
{

    cerr << "Const operator()(MV_VecIndex, MV_VecIndex) called " << endl;

    // check that index is not out of bounds
    //
    if (I.end() >= dim0_  || J.end() >= dim1_)
    {
        cerr << "Matrix index: (" << I.start() << ":" << I.end()  
             << "," << J.start() << ":" << J.end()   
             << ") not a subset of (0:" << dim0_ - 1 << ", 0:" 
             << dim1_-1 << ") " << endl;
        exit(1);
    }

    // this automatically returns a reference.  we need to 
    // "cast away" constness here, so the &v_[] arg will
    // not cause a compiler error.
    //
    MV_ColMat_complex *t =  (MV_ColMat_complex*) this;
    return MV_ColMat_complex(&(t->v_[J.start()*lda_ + I.start()]), 
            I.end() - I.start() + 1, 
            J.end() - J.start() + 1, lda_, MV_Matrix_::ref);
}

MV_ColMat_complex::~MV_ColMat_complex() {}

ostream&   operator<<(ostream& s, const MV_ColMat_complex& V)
{
    int M = V.size(0);
    int N = V.size(1);

    for (int i=0; i<M; i++)
    {
        for (int j=0; j<N; j++)
            s << V(i,j) << " " ;
        s << endl;
    }
    
    return s;
}



