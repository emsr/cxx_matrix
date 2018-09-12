#ifndef VEC2_MATH_H
#define VEC2_MATH_H 1

#include <cmath>

template<typename _Num>
  _Num
  cross(const vec2<_Num>& U, const vec2<_Num>& V)
  { return U[0] * V[1] - U[1] * V[0]; }

template<typename _Num>
  _Num
  angle(const vec2<_Num>& U, const vec2<_Num>& V)
  { std::atan2(cross(U, V), dot(U, V)); }

#endif // VEC2_MATH_H
