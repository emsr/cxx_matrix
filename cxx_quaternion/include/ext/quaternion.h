#ifndef QUATERNION_H
#define QUATERNION_H 1

#include <iosfwd>

namespace __gnu_cxx
{

  template<typename _Tp>
    class quaternion
    {

    public:

      explicit quaternion(_Tp q0 = 0.0);
      explicit quaternion(const _Tp Q[4]);
      quaternion(_Tp q0, _Tp q1, _Tp q2, _Tp q3);
      quaternion(_Tp w, const _Tp Q[3]);
      quaternion(const _Tp axis[3], _Tp angle, _Tp radius = 1.0);
      explicit quaternion(const _Tp m[4][4]);
      quaternion(const quaternion& Q) = default;
      quaternion(quaternion&& Q) = default;

      void set(_Tp q0);
      void set(const _Tp Q[4]);
      void set(_Tp q0, _Tp q1, _Tp q2, _Tp q3);
      void set(_Tp w, const _Tp Q[3]);
      void set(const _Tp axis[3], _Tp angle, _Tp radius = 1.0);
      void set(const _Tp m[4][4]);

      void get(_Tp Q[4]) const;
      void get(_Tp& q0, _Tp& q1, _Tp& q2, _Tp& q3) const;
      void get(_Tp& w, _Tp Q[3]) const;
      _Tp get(_Tp m[3][3]) const;
      void get(_Tp m[4][4]) const;
      void get(_Tp axis[3], _Tp& angle, _Tp& radius) const;

      _Tp operator[](std::size_t i) const;
      _Tp& operator[](std::size_t i);
      _Tp w() const;
      void w(_Tp w);
      _Tp x() const;
      void x(_Tp x);
      _Tp y() const;
      void y(_Tp y);
      _Tp z() const;
      void z(_Tp z);

      quaternion& add(const quaternion& Q);
      quaternion& subtract(const quaternion& Q);
      quaternion& multiply(_Tp c);
      quaternion& lmultiply(const quaternion& Q);
      quaternion& rmultiply(const quaternion& Q);
      quaternion& divide(_Tp c);
      quaternion& conjugate();
      quaternion& invert();
      quaternion& renormalize(_Tp c = 1.0);

      quaternion sum(const quaternion& Q) const;
      quaternion diff(const quaternion& Q) const;
      quaternion prod(_Tp c) const;
      quaternion lprod(const quaternion& Q) const;
      quaternion rprod(const quaternion& Q) const;
      quaternion quot(_Tp c) const;

      quaternion& operator=(_Tp q0);
      quaternion& operator+=(const quaternion& Q);
      quaternion& operator-=(const quaternion& Q);
      quaternion& operator*=(_Tp c);
      quaternion& operator*=(const quaternion& Q);
      quaternion& operator/=(_Tp c);
      quaternion& operator/=(const quaternion& Q);

      quaternion operator-() const;
      quaternion operator+() const;

      static const quaternion QZ, QU, QI, QJ, QK;

    protected:

      _Tp q[4];
    };

  template<typename _Tp>
    quaternion<_Tp> operator+(const quaternion<_Tp>& P,
			      const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> operator-(const quaternion<_Tp>& P,
			      const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> operator*(const quaternion<_Tp>& Q, _Tp c);
  template<typename _Tp>
    quaternion<_Tp> operator*(const quaternion<_Tp>& P,
			      const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> operator*(_Tp c, const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> operator/(const quaternion<_Tp>& Q, _Tp c);
  template<typename _Tp>
    quaternion<_Tp> operator/(const quaternion<_Tp>& P,
			      const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> operator/(_Tp c, const quaternion<_Tp>& Q);

  template<typename _Tp>
    quaternion<_Tp> conj(const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> inv(const quaternion<_Tp>& Q);
  template<typename _Tp>
    _Tp norm(const quaternion<_Tp>& Q);
  template<typename _Tp>
    _Tp abs(const quaternion<_Tp>& Q);
  template<typename _Tp>
    _Tp scalprod(const quaternion<_Tp>& P, const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> exp(const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> log(const quaternion<_Tp>& Q);
  template<typename _Tp>
    quaternion<_Tp> pow(const quaternion<_Tp>& Q, _Tp x);
  template<typename _Tp>
    quaternion<_Tp> slerp(const quaternion<_Tp>& P,
			  const quaternion<_Tp>& Q, _Tp t);
  template<typename _Tp>
    void rotate(const quaternion<_Tp>& Q, _Tp vec[3]);

  template<typename _Tp>
    bool operator==(const quaternion<_Tp>& P, const quaternion<_Tp>& Q);
  template<typename _Tp>
    bool operator!=(const quaternion<_Tp>& P, const quaternion<_Tp>& Q);

  template<typename CharT, typename Traits, typename _Tp>
    std::basic_istream<CharT, Traits>&
    operator>>(std::basic_istream<CharT, Traits>& is, quaternion<_Tp>& Q);
  template<typename CharT, typename Traits, typename _Tp>
    std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os, const quaternion<_Tp>& Q);

}

#include "quaternion.tcc"

#endif // QUATERNION_H
