//      MV++  (V. 1.2b Beta)
//      Numerical Matrix/MV_Vector Class Library
//      (c) 1994  Roldan Pozo
//


#include <math.h>
#include <stdlib.h>

#include "mv_vector_double.h"

MV_Vector_double& operator*=(MV_Vector_double &x, const double &a)
{
      int N = x.size();
      for (int i=0;i<N;i++)
         x(i) *= a;
      return x;
}

MV_Vector_double operator*(const double &a, const MV_Vector_double &x)
{
      int N = x.size();
      MV_Vector_double result(N);
      for (int i=0;i<N;i++)
         result(i) = x(i)*a;
      return result;
}

MV_Vector_double operator*(const MV_Vector_double &x, const double &a)
{
    // This is the other commutative case of vector*scalar.
    // It should be just defined to be
    // "return operator*(a,x);"
    // but some compilers (e.g. Turbo C++ v.3.0) have trouble
    // determining the proper template match.  For the moment,
    // we'll just duplicate the code in the scalar * vector 
    // case above.

      int N = x.size();
      MV_Vector_double result(N);
      for (int i=0;i<N;i++)
         result(i) = x(i)*a;
      return result;

}

MV_Vector_double operator+(const MV_Vector_double &x, const MV_Vector_double &y)
{
      int N = x.size();
      if (N != y.size())
      {
         cout << "Incompatible vector lengths in +." << endl;
         exit(1);
      }
      
      MV_Vector_double result(N);
      for (int i=0;i<N; i++)
         result(i) = x(i) + y(i);
      return result;
}
          
MV_Vector_double operator-(const MV_Vector_double &x, const MV_Vector_double &y)
{
      int N = x.size();
      if (N != y.size())
      {
         cout << "Incompatible vector lengths in -." << endl;
         exit(1);
      }
      
      MV_Vector_double result(N);
      for (int i=0;i<N; i++)
         result(i) = x(i) - y(i);
      return result;
}
          

MV_Vector_double& operator+=(MV_Vector_double &x, const MV_Vector_double &y)
{
      int N = x.size();
      if (N != y.size())
      {
         cout << "Incompatible vector lengths in -." << endl;
         exit(1);
      }
      
      for (int i=0;i<N; i++)
         x(i) += y(i);
      return x;
}
          
      
MV_Vector_double& operator-=(MV_Vector_double &x, const MV_Vector_double &y)
{
      int N = x.size();
      if (N != y.size())
      {
         cout << "Incompatible vector lengths in -." << endl;
         exit(1);
      }
      
      for (int i=0;i<N; i++)
         x(i) -= y(i);
      return x;
}
          
      

//  norm and dot product functions for the MV_Vector<> class


double dot(const MV_Vector_double &x, const MV_Vector_double &y)
{
        
  //  Check for compatible dimensions:
  if (x.size() != y.size())
      {
         cout << "Incompatible dimensions in dot(). " << endl;
         exit(1);
      }

      double temp =  0;
      for (int i=0; i<x.size();i++)
           temp += x(i)*y(i);
      return temp;
}

double norm(const MV_Vector_double &x)
{
      double temp = dot(x,x);
      return sqrt(temp);
}

