//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

//
//      mv_matrix.h  : basic templated numerical matrix class, storage
//                  by columns (Fortran oriented.)
//
//
//


#ifndef _MV_MATRIX_int_H_
#define _MV_MATRIX_int_H_    

#include "mv_vector_int.h"
#include "mv_matrix_ref.h"



#include <iostream.h>       // for formatted printing of matrices
#ifdef MV_MATRIX_BOUNDS_CHECK
#   include <assert.h>
#endif


class MV_ColMat_int
{                                                                      
    private:                                                           
           MV_Vector_int v_;
           int dim0_;   // perferred to using dim_[2]. some compilers
           int dim1_;   // refuse to initalize these in the constructor.
           int lda_;
           int ref_;   // true if this is declared as a reference vector,
                        // i.e. it does not own the memory space, but 
                        // rather it is a view to another vector or array.
    public:                                                            
                                                                       
        /*::::::::::::::::::::::::::*/                                 
        /* Constructors/Destructors */                                 
        /*::::::::::::::::::::::::::*/                                 
                                                                       
            MV_ColMat_int();                             
            MV_ColMat_int(unsigned int, unsigned int); 

    // some compilers have difficulty with inlined 'for' statements.
    MV_ColMat_int(unsigned int, unsigned int, const int&);   

    // usual copy by value
    // (can't use default parameter lda=m, because m is not a constant...)
    //
    MV_ColMat_int(int*, unsigned int m, unsigned int n);
    MV_ColMat_int(int*, unsigned int m, unsigned int n, unsigned int lda);

    // the "reference" versions
    //
    //
    MV_ColMat_int(int*, unsigned int m, unsigned int n, MV_Matrix_::ref_type i);
    MV_ColMat_int(int*, unsigned int m, unsigned int n, unsigned int lda,
                MV_Matrix_::ref_type i);

    MV_ColMat_int(const MV_ColMat_int&); 
    ~MV_ColMat_int();                              
                                                                       
        /*::::::::::::::::::::::::::::::::*/                           
        /*  Indices and access operations */                           
        /*::::::::::::::::::::::::::::::::*/                           
                                                                       
    inline int&     operator()(unsigned int, unsigned int); 
    inline const int&   operator()(unsigned int, unsigned int) const; 
    MV_ColMat_int operator()(const MV_VecIndex &I, const MV_VecIndex &J) ;
    const MV_ColMat_int operator()(const MV_VecIndex &I, const MV_VecIndex &J) const;
    unsigned int            size(int i) const; 
    MV_ColMat_int&      newsize(unsigned int, unsigned int);
    int ref() const { return ref_;}
                                                                       
        /*::::::::::::::*/                                             
        /*  Assignment  */                                             
        /*::::::::::::::*/                                             
                                                                       
    MV_ColMat_int & operator=(const MV_ColMat_int&);
    MV_ColMat_int & operator=(const int&);


    friend ostream& operator<<(ostream &s, const MV_ColMat_int &A);

};                                                                     

inline int& MV_ColMat_int::operator()(unsigned int i, unsigned int j)
{
#ifdef MV_MATRIX_BOUNDS_CHECK
    assert(0<=i && i<size(0));
    assert(0<=j && j<size(1));
#endif
    return v_(j*lda_ + i);      // could use indirect addressing
                                // instead...
}

inline const int& MV_ColMat_int::operator()
                    (unsigned int i, unsigned int j) const
{
#ifdef MV_MATRIX_BOUNDS_CHECK
    assert(0<=i && i<size(0));
    assert(0<=j && j<size(1));
#endif
    return v_(j*lda_ + i);
}

inline MV_ColMat_int::MV_ColMat_int(int* d, unsigned int m, unsigned int n,
        MV_Matrix_::ref_type i ):
            v_(d,m*n, MV_Vector_::ref), dim0_(m), dim1_(n), lda_(m), ref_(i) {}
 
inline MV_ColMat_int::MV_ColMat_int(int* d, unsigned int m, unsigned int n,
            unsigned int lda, MV_Matrix_::ref_type i) :
            v_(d, lda*n, MV_Vector_::ref), dim0_(m), dim1_(n), lda_(lda),
            ref_(i) {}

#endif

