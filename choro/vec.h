#ifndef VEC_H
#define VEC_H 1

#include <array>
#include <type_traits>

template<typename _Num, std::size_t _Dim>
  struct space_point
  : public std::array<_Num, _Dim>
  {
    static_assert(std::is_arithmetic_v<_Num>);

    // Origin.
    constexpr
    space_point()
    { this->fill(_Num{}); }

    constexpr
    space_point(std::initializer_list<_Num> __il)
    {
      const auto __n = std::min(_Dim, __il.size());
      std::copy(__il.begin(), __il.begin() + __n, this->begin());
      if (__n < _Dim)
	std::fill(this->begin() + __n, this->end(), _Num{});
    }
  };

template<typename _Num>
  using pt1 = space_point<_Num, 1u>;
using pt1f   = pt1<float>;
using pt1d   = pt1<double>;
using pt1l   = pt1<long double>;
using pt1s   = pt1<short>;
using pt1i   = pt1<int>;
using pt1li  = pt1<long>;
using pt1lli = pt1<long long>;

template<typename _Num>
  using pt2 = space_point<_Num, 2u>;
using pt2f   = pt2<float>;
using pt2d   = pt2<double>;
using pt2l   = pt2<long double>;
using pt2s   = pt2<short>;
using pt2i   = pt2<int>;
using pt2li  = pt2<long>;
using pt2lli = pt2<long long>;

template<typename _Num>
  using pt3 = space_point<_Num, 3u>;
using pt3f   = pt3<float>;
using pt3d   = pt3<double>;
using pt3l   = pt3<long double>;
using pt3s   = pt3<short>;
using pt3i   = pt3<int>;
using pt3li  = pt3<long>;
using pt3lli = pt3<long long>;

template<typename _Num>
  using pt4 = space_point<_Num, 4u>;
using pt4f   = pt4<float>;
using pt4d   = pt4<double>;
using pt4l   = pt4<long double>;
using pt4s   = pt4<short>;
using pt4i   = pt4<int>;
using pt4li  = pt4<long>;
using pt4lli = pt4<long long>;

template<typename _Num, std::size_t _Dim>
  struct space_vector
  : public std::array<_Num, _Dim>
  {
    static_assert(std::is_arithmetic_v<_Num>);

    // Null vector.
    constexpr
    space_vector()
    { this->fill(_Num{}); }

    constexpr
    space_vector(std::initializer_list<_Num> __il)
    {
      const auto __n = std::min(_Dim, __il.size());
      std::copy(__il.begin(), __il.begin() + __n, this->begin());
      if (__n < _Dim)
	std::fill(this->begin() + __n, this->end(), _Num{});
    }
 };

template<typename _Num>
  using vec1 = space_vector<_Num, 1u>;
using vec1f = vec1<float>;
using vec1d = vec1<double>;
using vec1l = vec1<long double>;
using vec1s = vec1<short>;
using vec1i = vec1<int>;
using vec1li = vec1<long>;
using vec1lli = vec1<long long>;

template<typename _Num>
  using vec2 = space_vector<_Num, 2u>;
using vec2f   = vec2<float>;
using vec2d   = vec2<double>;
using vec2l   = vec2<long double>;
using vec2s   = vec2<short>;
using vec2i   = vec2<int>;
using vec2li  = vec2<long>;
using vec2lli = vec2<long long>;

template<typename _Num>
  using vec3 = space_vector<_Num, 3u>;
using vec3f   = vec3<float>;
using vec3d   = vec3<double>;
using vec3l   = vec3<long double>;
using vec3s   = vec3<short>;
using vec3i   = vec3<int>;
using vec3li  = vec3<long>;
using vec3lli = vec3<long long>;

template<typename _Num>
  using vec4 = space_vector<_Num, 4u>;
using vec4f   = vec4<float>;
using vec4d   = vec4<double>;
using vec4l   = vec4<long double>;
using vec4s   = vec4<short>;
using vec4i   = vec4<int>;
using vec4li  = vec4<long>;
using vec4lli = vec4<long long>;

template<typename _Num, std::size_t _NRows, std::size_t _NCols>
  struct space_matrix
  : public space_vector<space_vector<_Num, _NRows>, _NCols>
  {
    static_assert(std::is_arithmetic_v<_Num>);
  };

template<typename _Num>
  using mat2x2 = space_matrix<_Num, 2u, 2u>;
using mat2x2f   = mat2x2<float>;
using mat2x2d   = mat2x2<double>;
using mat2x2l   = mat2x2<long double>;
using mat2x2s   = mat2x2<short>;
using mat2x2i   = mat2x2<int>;
using mat2x2li  = mat2x2<long>;
using mat2x2lli = mat2x2<long long>;

template<typename _Num>
  using mat2x3 = space_matrix<_Num, 2u, 3u>;
using mat2x3f   = mat2x3<float>;
using mat2x3d   = mat2x3<double>;
using mat2x3l   = mat2x3<long double>;
using mat2x3s   = mat2x3<short>;
using mat2x3i   = mat2x3<int>;
using mat2x3li  = mat2x3<long>;
using mat2x3lli = mat2x3<long long>;

template<typename _Num>
  using mat2x4 = space_matrix<_Num, 2u, 4u>;
using mat2x4f   = mat2x4<float>;
using mat2x4d   = mat2x4<double>;
using mat2x4l   = mat2x4<long double>;
using mat2x4s   = mat2x4<short>;
using mat2x4i   = mat2x4<int>;
using mat2x4li  = mat2x4<long>;
using mat2x4lli = mat2x4<long long>;

template<typename _Num>
  using mat3x2 = space_matrix<_Num, 3u, 2u>;
using mat3x2f   = mat3x2<float>;
using mat3x2d   = mat3x2<double>;
using mat3x2l   = mat3x2<long double>;
using mat3x2s   = mat3x2<short>;
using mat3x2i   = mat3x2<int>;
using mat3x2li  = mat3x2<long>;
using mat3x2lli = mat3x2<long long>;

template<typename _Num>
  using mat3x3 = space_matrix<_Num, 3u, 3u>;
using mat3x3f   = mat3x3<float>;
using mat3x3d   = mat3x3<double>;
using mat3x3l   = mat3x3<long double>;
using mat3x3s   = mat3x3<short>;
using mat3x3i   = mat3x3<int>;
using mat3x3li  = mat3x3<long>;
using mat3x3lli = mat3x3<long long>;

template<typename _Num>
  using mat3x4 = space_matrix<_Num, 3u, 4u>;
using mat3x4f   = mat3x4<float>;
using mat3x4d   = mat3x4<double>;
using mat3x4l   = mat3x4<long double>;
using mat3x4s   = mat3x4<short>;
using mat3x4i   = mat3x4<int>;
using mat3x4li  = mat3x4<long>;
using mat3x4lli = mat3x4<long long>;

template<typename _Num>
  using mat4x2 = space_matrix<_Num, 4u, 2u>;
using mat4x2f   = mat4x2<float>;
using mat4x2d   = mat4x2<double>;
using mat4x2l   = mat4x2<long double>;
using mat4x2s   = mat4x2<short>;
using mat4x2i   = mat4x2<int>;
using mat4x2li  = mat4x2<long>;
using mat4x2lli = mat4x2<long long>;

template<typename _Num>
  using mat4x3 = space_matrix<_Num, 4u, 3u>;
using mat4x3f   = mat4x3<float>;
using mat4x3d   = mat4x3<double>;
using mat4x3l   = mat4x3<long double>;
using mat4x3s   = mat4x3<short>;
using mat4x3i   = mat4x3<int>;
using mat4x3li  = mat4x3<long>;
using mat4x3lli = mat4x3<long long>;

template<typename _Num>
  using mat4x4 = space_matrix<_Num, 4u, 4u>;
using mat4x4f   = mat4x4<float>;
using mat4x4d   = mat4x4<double>;
using mat4x4l   = mat4x4<long double>;
using mat4x4s   = mat4x4<short>;
using mat4x4i   = mat4x4<int>;
using mat4x4li  = mat4x4<long>;
using mat4x4lli = mat4x4<long long>;

template<typename _Num, std::size_t _Dim>
  struct box
  : public std::array<space_point<_Num, _Dim>, 2u>
  {
    static_assert(std::is_arithmetic_v<_Num>);
  };

template<typename _Num>
  using box1 = box<_Num, 1u>;
using box1f   = box1<float>;
using box1d   = box1<double>;
using box1l   = box1<long double>;
using box1s   = box1<short>;
using box1i   = box1<int>;
using box1li  = box1<long>;
using box1lli = box1<long long>;

template<typename _Num>
  using box2 = box<_Num, 2u>;
using box2f   = box2<float>;
using box2d   = box2<double>;
using box2l   = box2<long double>;
using box2s   = box2<short>;
using box2i   = box2<int>;
using box2li  = box2<long>;
using box2lli = box2<long long>;

template<typename _Num>
  using box3 = box<_Num, 3u>;
using box3f   = box3<float>;
using box3d   = box3<double>;
using box3l   = box3<long double>;
using box3s   = box3<short>;
using box3i   = box3<int>;
using box3li  = box3<long>;
using box3lli = box3<long long>;

template<typename _Num>
  using box4 = box<_Num, 4u>;
using box4f   = box4<float>;
using box4d   = box4<double>;
using box4l   = box4<long double>;
using box4s   = box4<short>;
using box4i   = box4<int>;
using box4li  = box4<long>;
using box4lli = box4<long long>;

#endif // VEC_H
