#ifndef VEC_MATH_H
#define VEC_MATH_H 1

// end - begin?
// Should also be a space_vector ctor.
template<typename _Num, std::size_t _Dim>
  constexpr space_vector<_Num, _Dim>
  operator-(const space_point<_Num, _Dim>& Q,
	    const space_point<_Num, _Dim>& P)
  {
    space_vector<_Num, _Dim> V;
    for (unsigned int c = 0; c < _Dim; ++c)
      V[c] = Q[c] - P[c];
    return V;
  }

// Space vector.

template<typename _Num, std::size_t _Dim>
  constexpr space_vector<_Num, _Dim>
  operator-(const space_vector<_Num, _Dim>& U,
	    const space_vector<_Num, _Dim>& V)
  {
    space_vector<_Num, _Dim> W{U};
    for (unsigned int c = 0; c < _Dim; ++c)
      W[c] -=  V[c];
    return W;
  }

template<typename _Num, std::size_t _Dim>
  constexpr space_vector<_Num, _Dim>
  operator+(const space_vector<_Num, _Dim>& U,
	    const space_vector<_Num, _Dim>& V)
  {
    space_vector<_Num, _Dim> W{U};
    for (unsigned int c = 0; c < _Dim; ++c)
      W[c] += V[c];
    return W;
  }

template<typename _Num, std::size_t _Dim>
  constexpr space_vector<_Num, _Dim>
  operator*(_Num s, const space_vector<_Num, _Dim>& V)
  {
    space_vector<_Num, _Dim> W{V};
    for (unsigned int c = 0; c < _Dim; ++c)
      W[c] *= s;
    return W;
  }

template<typename _Num, std::size_t _Dim>
  constexpr space_vector<_Num, _Dim>
  operator*(const space_vector<_Num, _Dim>& V, _Num s)
  {
    space_vector<_Num, _Dim> W{V};
    for (unsigned int c = 0; c < _Dim; ++c)
      W[c] *= s;
    return W;
  }

template<typename _Num, std::size_t _Dim>
  constexpr _Num
  dot(const space_vector<_Num, _Dim>& U,
      const space_vector<_Num, _Dim>& V)
  {
    _Num d{};
    for (unsigned int c = 0; c < _Dim; ++c)
      d += U[c] * V[c];
    return d;
  }

template<typename _Num, std::size_t _Dim>
  constexpr space_vector<_Num, _Dim>&
  scale(_Num s, space_vector<_Num, _Dim>& V)
  {
    for (unsigned int c = 0; c < _Dim; ++c)
      V[c] *= s;
    return V;
  }

#include "vec2_math.h"

#include "vec3_math.h"

#endif // VEC_MATH_H
