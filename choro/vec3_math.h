#ifndef VEC3_MATH_H
#define VEC3_MATH_H 1

template<typename _Num>
  constexpr vec3<_Num>
  cross(const vec3<_Num>& U, const vec3<_Num>& V)
  {
    vec3<_Num> W;
    for (int c = 0; c < 3; ++c)
      W[c] += U[(c+1)%3] * V[(c+2)%3] - U[(c+2)%3] * V[(c+1)%3];
    return W;
  }

#endif // VEC3_MATH_H
