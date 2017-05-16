//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//

//
//      mv_vector.h     Basic templated vector class
//

#ifndef _MV_VECTOR_H_
#define _MV_VECTOR_H_    

//
//      Key features:
//
//    o efficient indexing as fast as native C arrays 
//    o supports only unit strides (for efficient indexing)
//    o templated data elements
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
//      o  The MV_Vector(*TYPE, int len) constructor is now a deep-copy to
//      match the MV_Vector(const &MV_Vector) constructor.  To create a view
//      (share semantics) use
//
//          MV_Vector<double> A( &d[0], n, MV_Vector_::ref );
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
#include "mv_vector_ref.h"

template <class TYPE>
class MV_Vector
{                                                                      
    protected:                                                           
           TYPE *p_;
           unsigned int dim_;
           int ref_;  // 0 or 1; does this own its own memory space?
    public:                                                            


        /*::::::::::::::::::::::::::*/                                 
        /* Constructors/Destructors */                                 
        /*::::::::::::::::::::::::::*/                                 
                                                                       
    MV_Vector();                             
    MV_Vector(unsigned int);                             
    MV_Vector(unsigned int, const TYPE&);   // can't be inlined because of 'for'
                                       // statement.
    MV_Vector(TYPE*, unsigned int);     // new copy
    MV_Vector(const TYPE*, unsigned int);       // new copy ???
    
    // reference of an exisiting data structure
    //
    MV_Vector(TYPE*, unsigned int, MV_Vector_::ref_type i); 
    MV_Vector(const MV_Vector<TYPE>&); 
    ~MV_Vector();                              
                                                                       
        /*::::::::::::::::::::::::::::::::*/                           
        /*  Indices and access operations */                           
        /*::::::::::::::::::::::::::::::::*/                           
                                                                       

    // code for operator() is defined here, otherwise some compilers 
    // (e.g. Turbo C++ v 3.0) cannot inline them properly...
    //
    inline            TYPE&     operator()(unsigned int i)
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }
    inline  const  TYPE&    operator()(unsigned int i) const 
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }

    inline        TYPE&     operator[](unsigned int i)
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }
    inline      const  TYPE&    operator[](unsigned int i) const 
                  {
#                   ifdef MV_VECTOR_BOUNDS_CHECK
                    assert(i < dim_);
#                   endif
                    return p_[i];
                  }



    inline MV_Vector<TYPE> operator()(const MV_VecIndex &I) ;
    inline MV_Vector<TYPE> operator()(void);
    inline const MV_Vector<TYPE> operator()(void) const;
    inline const MV_Vector<TYPE> operator()(const MV_VecIndex &I) const;
//
//   the following line causes ambiguatities with template instantiations
//   should be avoid.  Used &v(0) explicitly when converting to TYPE*.
//
//    inline                operator const  TYPE*() const {return p_;} 
    inline unsigned int             size() const { return dim_;}
    inline int                      ref() const { return  ref_;}
    inline int                      null() const {return dim_== 0;}
            //
            // Create a new *uninitalized* vector of size N
            MV_Vector<TYPE> & newsize(unsigned int );
                                                                       
        /*::::::::::::::*/                                             
        /*  Assignment  */                                             
        /*::::::::::::::*/                                             
                                                                       
            MV_Vector<TYPE> & operator=(const MV_Vector<TYPE>&);
            MV_Vector<TYPE> & operator=(const TYPE&);


    friend ostream& operator<<(ostream &s, const MV_Vector<TYPE> &A);

};                                                                     

    
template <class TYPE>
MV_Vector<TYPE>::MV_Vector()  : p_(0), dim_(0) , ref_(0){};

template <class TYPE>
MV_Vector<TYPE>::MV_Vector(unsigned int n) : p_(new TYPE[n]), dim_(n), 
            ref_(0)
{
    if (p_ == NULL)
    {
        cerr << "Error: NULL pointer in MV_Vector(int) constructor " << endl;
        cerr << "       Most likely out of memory... " << endl;
        exit(1);
    }
}

template <class TYPE>
MV_Vector<TYPE>::MV_Vector(unsigned int n, const TYPE& v) : 
        p_(new TYPE[n]), dim_(n), ref_(0)
{
    if (p_ == NULL)
    {
        cerr << "Error: NULL pointer in MV_Vector(int) constructor " << endl;
        cerr << "       Most likely out of memory... " << endl;
        exit(1);
    }
    for (int i=0; i<n; i++)
        p_[i] = v;
}

// operators and member functions
//




template <class TYPE>
MV_Vector<TYPE>& MV_Vector<TYPE>::operator=(const TYPE & m) 
{
#ifdef TRACE_VEC
    cout << "> MV_Vector<TYPE>::operator=(const TYPE & m)  " << endl;
#endif

    // unroll loops to depth of length 4

    int N = size();

    int Nminus4 = N-4;
    int i;

    for (i=0; i<Nminus4; )
    {
        p_[i++] = m;
        p_[i++] = m;
        p_[i++] = m;
        p_[i++] = m;
    }

    for (; i<N; p_[i++] = m);   // finish off last piece...

#ifdef TRACE_VEC
    cout << "< MV_Vector<TYPE>::operator=(const TYPE & m)  " << endl;
#endif
    return *this;
}

template <class TYPE>
MV_Vector<TYPE>& MV_Vector<TYPE>::newsize(unsigned int n)
{
#ifdef TRACE_VEC
    cout << "> MV_Vector<TYPE>::newsize(unsigned int n) " << endl;
#endif
    if (ref_ )                  // is this structure just a pointer?
    {
        {
            cerr << "MV_Vector::newsize can't operator on references.\n";
            exit(1);
        }
    }
    else
    if (dim_ != n )                     // only delete and new if
    {                                   // the size of memory is really
        if (p_) delete [] p_;           // changing, otherwise just
        p_ = new TYPE[n];               // copy in place.
        if (p_ == NULL)
        {
            cerr << "Error : NULL pointer in operator= " << endl;
            exit(1);
        }
        dim_ = n;
    }

#ifdef TRACE_VEC
    cout << "< MV_Vector<TYPE>::newsize(unsigned int n) " << endl;
#endif

    return *this;
}


    

template <class TYPE>
MV_Vector<TYPE>& MV_Vector<TYPE>::operator=(const MV_Vector<TYPE> & m) 
{

    int N = m.dim_;
    int i;

    if (ref_ )                  // is this structure just a pointer?
    {
        if (dim_ != m.dim_)     // check conformance,
        {
            cerr << "MV_VectorRef::operator=  non-conformant assignment.\n";
            exit(1);
        }

        // handle overlapping matrix references
        if ((m.p_ + m.dim_) >= p_)
        {
            // overlap case, copy backwards to avoid overwriting results
            for (i= N-1; i>=0; i--)
                p_[i] = m.p_[i];
        }
        else
        {
            for (i=0; i<N; i++)
                p_[i] = m.p_[i];
        }
                
    }
    else
    {
        newsize(N);

        // no need to test for overlap, since this region is new
        for (i =0; i< N; i++)       // careful not to use bcopy()
            p_[i] = m.p_[i];                // here, but TYPE::operator= TYPE.
    }
    return *this;   
}

template <class TYPE>
MV_Vector<TYPE>::MV_Vector(const MV_Vector<TYPE> & m) : p_(new TYPE[m.dim_]), 
    dim_(m.dim_) , ref_(0)
{
    if (p_ == NULL)
    {
        cerr << "Error:  Null pointer in MV_Vector(const MV_Vector&); " << endl;
        exit(1);
    }

    int N = m.dim_;

    for (int i=0; i<N; i++)
        p_[i] = m.p_[i];
}

// note that ref() is initalized with i rather than 1.
// this is so compilers will not generate a warning that i was
// not used in the construction.  (MV_Vector::ref_type is an enum that
// can *only* have the value of 1.
//
template <class TYPE>
MV_Vector<TYPE>::MV_Vector(TYPE* d, unsigned int n, MV_Vector_::ref_type i) : 
        p_(d), dim_(n) , ref_(i) {}

template <class TYPE>
MV_Vector<TYPE>::MV_Vector(TYPE* d, unsigned int n) : p_(new TYPE[n]), 
      dim_(n) , ref_(0)
{
    if (p_ == NULL)
    {
        cerr << "Error: Null pointer in MV_Vector(TYPE*, int) " << endl;
        exit(1);
    }
    for (int i=0; i<n; i++)
        p_[i] = d[i];

}


template <class TYPE>
MV_Vector<TYPE>::MV_Vector(const TYPE* d, unsigned int n) : p_(new TYPE[n]), 
      dim_(n) , ref_(0)
{
    if (p_ == NULL)
    {
        cerr << "Error: Null pointer in MV_Vector(TYPE*, int) " << endl;
        exit(1);
    }
    for (int i=0; i<n; i++)
        p_[i] = d[i];

}

template <class TYPE>
MV_Vector<TYPE> MV_Vector<TYPE>::operator()(void)
{
    return MV_Vector<TYPE>(p_, dim_, MV_Vector_::ref);
}

template <class TYPE>
const MV_Vector<TYPE> MV_Vector<TYPE>::operator()(void) const
{
    return MV_Vector<TYPE>(p_, dim_, MV_Vector_::ref);
}

template <class TYPE>
MV_Vector<TYPE> MV_Vector<TYPE>::operator()(const MV_VecIndex &I) 
{
    // default parameters
    if (I.all())
        return MV_Vector<TYPE>(p_, dim_, MV_Vector_::ref);
    else
    {
    // check that index is not out of bounds
    //
        if ( I.end() >= dim_)
        {
            cerr << "MV_VecIndex: (" << I.start() << ":" << I.end() << 
                ") too big for matrix (0:" << dim_ - 1 << ") " << endl;
            exit(1);
        }
        return MV_Vector<TYPE>(p_+ I.start(), I.end() - I.start() + 1,
            MV_Vector_::ref);
    }
}

template <class TYPE>
const MV_Vector<TYPE> MV_Vector<TYPE>::operator()(const MV_VecIndex &I) const
{
    // check that index is not out of bounds
    //
    if ( I.end() >= dim_)
    {
        cerr << "MV_VecIndex: (" << I.start() << ":" << I.end() << 
                ") too big for matrix (0:" << dim_ - 1 << ") " << endl;
        exit(1);
    }
    return MV_Vector<TYPE>(p_+ I.start(), I.end() - I.start() + 1,
            MV_Vector_::ref);
}

template <class TYPE>
MV_Vector<TYPE>::~MV_Vector()
{
        if (p_ && !ref_ ) delete [] p_;
}

template <class TYPE>
ostream&   operator<<(ostream& s, const MV_Vector<TYPE>& V)
{
    int N = V.size();

    for (int i=0; i< N; i++)
        s << V(i) << endl;
    
    return s;
}


template <class TYPE>
class FMV_Vector : public MV_Vector<TYPE>
{                                                                      
    public:                                                            
        FMV_Vector(unsigned int n) : MV_Vector<TYPE>(n) {}
        FMV_Vector<TYPE>& operator=(const FMV_Vector<TYPE>& m);
        FMV_Vector<TYPE>& operator=(const TYPE& m);
};

template <class TYPE>
FMV_Vector<TYPE>& FMV_Vector<TYPE>::operator=( const FMV_Vector<TYPE>& m)
{

#ifdef TRACE_VEC
    cout << "> FMV_Vector<TYPE>::operator=( const FMV_Vector<TYPE>& m)" << endl;
#endif

    int N = m.dim_;



    if (ref_ )                  // is this structure just a pointer?
    {
        if (dim_ != m.dim_)     // check conformance,
        {
            cerr << "MV_VectorRef::operator=  non-conformant assignment.\n";
            exit(1);
        }
    }
    else if ( dim_ != m.dim_ )      // resize only if necessary
        newsize(N);

    memmove(p_, m.p_, N * sizeof(TYPE));

#ifdef TRACE_VEC
    cout << "< FMV_Vector<TYPE>::operator=( const FMV_Vector<TYPE>& m)" << endl;
#endif

    return *this;   
}

template <class TYPE>
FMV_Vector<TYPE>& FMV_Vector<TYPE>::operator=(const TYPE & m) 
{
#ifdef TRACE_VEC
    cout << "> FMV_Vector<TYPE>::operator=(const TYPE & m)  " << endl;
#endif

    // unroll loops to depth of length 4

    int N = size();

    int Nminus4 = N-4;
    int i;

    for (i=0; i<Nminus4; )
    {
        p_[i++] = m;
        p_[i++] = m;
        p_[i++] = m;
        p_[i++] = m;
    }

    for (; i<N; p_[i++] = m);   // finish off last piece...

#ifdef TRACE_VEC
    cout << "< FMV_Vector<TYPE>::operator=(const TYPE & m)  " << endl;
#endif
    return *this;
}

#include "mv_blas1.h"

#endif 
// _MV_VECTOR_H_

