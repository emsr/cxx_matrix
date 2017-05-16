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


#ifndef _MV_MATRIX_double_H_
#define _MV_MATRIX_double_H_    

#include "mv_vector_double.h"
#include "mv_matrix_ref.h"



#include <iostream.h>       // for formatted printing of matrices
#ifdef MV_MATRIX_BOUNDS_CHECK
#   include <assert.h>
#endif


class MV_ColMat_double
{                                                                      
    private:                                                           
           MV_Vector_double v_;
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
                                                                       
            MV_ColMat_double();                             
            MV_ColMat_double(unsigned int, unsigned int); 

    // some compilers have difficulty with inlined 'for' statements.
    MV_ColMat_double(unsigned int, unsigned int, const double&);   

    // usual copy by value
    // (can't use default parameter lda=m, because m is not a constant...)
    //
    MV_ColMat_double(double*, unsigned int m, unsigned int n);
    MV_ColMat_double(double*, unsigned int m, unsigned int n, unsigned int lda);

    // the "reference" versions
    //
    //
    MV_ColMat_double(double*, unsigned int m, unsigned int n, MV_Matrix_::ref_type i);
    MV_ColMat_double(double*, unsigned int m, unsigned int n, unsigned int lda,
                MV_Matrix_::ref_type i);

    MV_ColMat_double(const MV_ColMat_double&); 
    ~MV_ColMat_double();                              
                                                                       
        /*::::::::::::::::::::::::::::::::*/                           
        /*  Indices and access operations */                           
        /*::::::::::::::::::::::::::::::::*/                           
                                                                       
    inline double&      operator()(unsigned int, unsigned int); 
    inline const double&    operator()(unsigned int, unsigned int) const; 
    MV_ColMat_double operator()(const MV_VecIndex &I, const MV_VecIndex &J) ;
    const MV_ColMat_double operator()(const MV_VecIndex &I, const MV_VecIndex &J) const;
    unsigned int            size(int i) const; 
    MV_ColMat_double&       newsize(unsigned int, unsigned int);
    int ref() const { return ref_;}
                                                                       
        /*::::::::::::::*/                                             
        /*  Assignment  */                                             
        /*::::::::::::::*/                                             
                                                                       
    MV_ColMat_double & operator=(const MV_ColMat_double&);
    MV_ColMat_double & operator=(const double&);


    friend ostream& operator<<(ostream &s, const MV_ColMat_double &A);

};                                                                     

inline double& MV_ColMat_double::operator()(unsigned int i, unsigned int j)
{
#ifdef MV_MATRIX_BOUNDS_CHECK
    assert(0<=i && i<size(0));
    assert(0<=j && j<size(1));
#endif
    return v_(j*lda_ + i);      // could use indirect addressing
                                // instead...
}

inline const double& MV_ColMat_double::operator()
                    (unsigned int i, unsigned int j) const
{
#ifdef MV_MATRIX_BOUNDS_CHECK
    assert(0<=i && i<size(0));
    assert(0<=j && j<size(1));
#endif
    return v_(j*lda_ + i);
}

inline MV_ColMat_double::MV_ColMat_double(double* d, unsigned int m, unsigned int n,
        MV_Matrix_::ref_type i ):
            v_(d,m*n, MV_Vector_::ref), dim0_(m), dim1_(n), lda_(m), ref_(i) {}
 
inline MV_ColMat_double::MV_ColMat_double(double* d, unsigned int m, unsigned int n,
            unsigned int lda, MV_Matrix_::ref_type i) :
            v_(d, lda*n, MV_Vector_::ref), dim0_(m), dim1_(n), lda_(lda),
            ref_(i) {}

#endif

