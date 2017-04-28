
template<std::size_t Dim, typename Tp>
  struct vec
  {
  };


template<typename Tp>
  using vec2 = vec<2, Tp>;

using vec2f = vec2<float>;
using vec2d = vec2<double>;
using vec2l = vec2<long double>;


template<typename Tp>
  using vec3 = vec<3, Tp>;

using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3l = vec3<long double>;


template<typename Tp>
  using vec4 = vec<4, Tp>;

using vec4f = vec4<float>;
using vec4d = vec4<double>;
using vec4l = vec4<long double>;
