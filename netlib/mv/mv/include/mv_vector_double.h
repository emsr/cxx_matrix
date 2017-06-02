//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

//
//      mv_vector_double.h      Basic vector class (double precision)
//

#ifndef _MV_VECTOR_double_H
#define _MV_VECTOR_double_H    

//
//      Key features:
//
//    o efficient indexing as fast as native C arrays 
//    o supports only unit strides (for efficient indexing)
//    o copy-by-value semantics
//    o optional "share" semantics allows vectors to be constructed as
//          "views", or "references" of an existing memory, using 
//          MV_Vector_::ref modifier in the constuctor.  (see note below.)
//    o vector views can assign and references sections of vector, but
//          cannot modify their *size*.
//    o block-range indexing via MV_VecIndex class (e.g. A(I) = B; )
//          (note for the above to work, A(I) must return a vector view.)
//    o optional range checking (compile switch)
//    o fast copying (A=B) via loop unrolling
//    o (experimental derived FMV_Vector class) for even faster copying via 
//          memcpy() for data elements employing simple bit-wise copying (e.g.
//          float, complex, et.c)
//    o support for both [] and () style indexing  ([] not available 
//          for matrices.)
//
//  NOTES:
//
//      o  O(N) loops for copying and assigning scalars to vectors unroll
//      loops to a depth of 4.  Thus on some machines, it is faster
//      to execute A=scalar, than to manually assign a native C
//      array using an explicit for loop:
//      
//          for (i=0; i<N; d[i++] = scalar);
//
//      o   function code for the () and [] operators has been 
//      inlined into the class declaration, for compilers
//      (e.g. Turbo C++ v. 3.0) that refuse to inline otherwise.
//
//      o  The MV_Vector(*double, int len) constructor is now a deep-copy to
//      match the MV_Vector(const &MV_Vector) constructor.  To create a view
//      (share semantics) use
//
//          MV_Vector_double A( &d[0], n, MV_Vector_::ref );
//
//      This allows one to construct vectors as views of any contiguous C
//      array.  It will not destruct the memory space when the vector
//      is destroyed or goes out of scope.
//

                                 



#include <iostream.h>       // for formatted printing of matrices

#ifdef MV_VECTOR_BOUNDS_CHECK
#   include <assert.h>
#endif

#include "mv_vecindex.h"

// this is really used as a sort of global constant. The reason
// for creating its own type is that so it can be overloaded to perform
// a deep or shallow assignement.  (Any variable of type MV_Vector_::ref_type
// has only one possible value: one.)

#include "mv_vector_ref.h"

class MV_Vector_double
{                                                                      
    protected:                                                           
           double *p_;
           unsigned int dim_;
           int ref_;  // 0 or 1; does this own its own memory space?
    public:                                                            


        /*::::::::::::::::::::::::::*/                                 
        /* Constructors/Destructors */                                 
        /*::::::::::::::::::::::::::*/                                 
                                                                       
    MV_Vector_double();                             
    MV_Vector_double(unsigned int);                             
    MV_Vector_double(unsigned int, const double&);   // can't be inlined 
                                                     //because of 'for'
                                                    // statement.
    MV_Vector_double(double*, unsigned int);        // new copy
    MV_Vector_double(const double*, unsigned int);      // new copy ???
    
    // reference of an exisiting data structure
    //
    MV_Vector_double(double*, unsigned int, MV_Vector_::ref_type i);    
    MV_Vector_double(const MV_Vector_double &); 
    ~MV_Vector_double();                              
                                                                       
        /*::::::::::::::::::::::::::::::::*/                           
        /*  Indices and access operations */                           
        /*::::::::::::::::::::::::::::::::*/                           
                                                                       

    // code for operator() is defined here, otherwise some compilers 
    // (e.g. Turbo C++ v 3.0) cannot inline them properly...
    //
    double&     operator()(unsigned int i)
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }
    const  double&      operator()(unsigned int i) const 
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }

    double&     operator[](unsigned int i)
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }
    const  double&      operator[](unsigned int i) const 
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }



    MV_Vector_double operator()(const MV_VecIndex &I) ;
    MV_Vector_double operator()(void);
    const MV_Vector_double operator()(void) const;
    const MV_Vector_double operator()(const MV_VecIndex &I) const;
//
//   the following line causes ambiguatities with template instantiations
//   should be avoid.  Used &v(0) explicitly when converting to double*.
//
//    inline                operator const  double*() const {return p_;} 
    inline unsigned int             size() const { return dim_;}
    inline int                      ref() const { return  ref_;}
    inline int                      null() const {return dim_== 0;}
            //
            // Create a new *uninitalized* vector of size N
            MV_Vector_double & newsize(unsigned int );
                                                                       
        /*::::::::::::::*/                                             
        /*  Assignment  */                                             
        /*::::::::::::::*/                                             
                                                                       
            MV_Vector_double & operator=(const MV_Vector_double&);
            MV_Vector_double & operator=(const double&);


    friend ostream& operator<<(ostream &s, const MV_Vector_double &A);

};                                                                     

#include "mv_blas1_double.h"

#endif  
