#ifndef QUATERNION_TCC
#define QUATERNION_TCC 1

#include <cmath>
#include <iostream>
#include <sstream>

namespace __gnu_cxx
{

/**
 *  Default constructor.
 */
template<typename _Tp>
  quaternion<_Tp>::quaternion(_Tp q0)
  { this->set(q0); }

/**
 *  Constructor taking an array of four _Tps representing the components of the quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>::quaternion(const _Tp Q[4])
  { this->set(Q); }

/**
 *  Constructor taking four _Tp scalars representing the components of the quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>::quaternion(_Tp q0, _Tp q1, _Tp q2, _Tp q3)
  { this->set(q0, q1, q2, q3); }

/**
 *  Constructor taking a _Tp scalar and a _Tp 3-vector.
 */
template<typename _Tp>
  quaternion<_Tp>::quaternion(_Tp w, const _Tp Q[3])
  { this->set(w, Q); }

/**
 *  Constructor taking and a _Tp 3-vector a _Tp scalar representing a rotation axis
 *  and a rotation angle in radians respectively.
 */
template<typename _Tp>
  quaternion<_Tp>::quaternion(const _Tp axis[3], _Tp angle, _Tp radius)
  {  set(axis, angle, radius); }

/**
 *  Constructor taking a _Tp 4x4 transform matrix of the form
 *
 *  | Mxx  Mxy  Mxz  0 |
 *  | Myx  Myy  Myz  0 |
 *  | Mzx  Mzy  Mzz  0 |
 *  |  0    0    0   1 |
 */
template<typename _Tp>
  quaternion<_Tp>::quaternion(const _Tp m[4][4])
  { this->set(m); }

/**
 *  Set the quaternion<_Tp> from a real scalar.
 */
template<typename _Tp>
  void
  quaternion<_Tp>::set(_Tp q0)
  {
    this->q[0] = q0;
    this->q[1] = 0.0;
    this->q[2] = 0.0;
    this->q[3] = 0.0;
  }

/**
 *  Set the quaternion<_Tp> from an array of four _Tps representing the components of the quaternion<_Tp>.
 */
template<typename _Tp>
  void
  quaternion<_Tp>::set(const _Tp Q[4])
  {
    this->q[0] = Q[0];
    this->q[1] = Q[1];
    this->q[2] = Q[2];
    this->q[3] = Q[3];
  }

/**
 *  Set the quaternion<_Tp> from four _Tp scalars representing the components of the quaternion<_Tp>.
 */
template<typename _Tp>
  void
  quaternion<_Tp>::set(_Tp q0, _Tp q1, _Tp q2, _Tp q3)
  {
    this->q[0] = q0;
    this->q[1] = q1;
    this->q[2] = q2;
    this->q[3] = q3;
  }

/**
 *  Set the quaternion from a scalar and a vector.
 */
template<typename _Tp>
  void
  quaternion<_Tp>::set(_Tp w, const _Tp Q[3])
  {
    this->q[0] = w;
    this->q[1] = Q[0];
    this->q[2] = Q[1];
    this->q[3] = Q[2];
  }

/**
 *  Set the quaternion<_Tp> from a 3-vector a scalar representing a rotation axis
 *  and a rotation angle in radians respectively.
 */
template<typename _Tp>
  void
  quaternion<_Tp>::set(const _Tp axis[3], _Tp angle, _Tp radius)
  {
    const auto alen = std::sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
    if (alen < 1.0e-16)
      return;

    const auto afact = radius * std::sin(0.5 * angle) / alen;
    this->q[0] = radius * std::cos(0.5 * angle);
    this->q[1] = axis[0] * afact;
    this->q[2] = axis[1] * afact;
    this->q[3] = axis[2] * afact;
  }

/**
 *  Set the quaternion<_Tp> from a _Tp 4x4 transform matrix of the form
 *
 *  | Mxx  Mxy  Mxz  0 |
 *  | Myx  Myy  Myz  0 |
 *  | Mzx  Mzy  Mzz  0 |
 *  |  0    0    0   1 |
 */
template<typename _Tp>
  void
  quaternion<_Tp>::set(const _Tp m[4][4])
  {
    const auto t = 1.0 + m[0][0] + m[1][1] + m[2][2];  //  The trace of the matrix.
    if (std::abs(t) > 1.0e-16)
      {
	const auto s = 2.0 * std::sqrt(t);
	this->q[0] = 0.25 * s;
	this->q[1] = (m[2][1] - m[1][2]) / s;
	this->q[2] = (m[0][2] - m[2][0]) / s;
	this->q[3] = (m[1][0] - m[0][1]) / s;
      }
    else
      {
	int  max = 0;
	if (m[1][1] > m[max][max])
	  max = 1;
	if (m[2][2] > m[max][max])
	  max = 2;

	if (max == 0)
	  {
	    const auto s = std::sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]);
	    this->q[0] = 0.25 * s;
	    this->q[1] = (m[1][0] + m[0][1]) / s;
	    this->q[2] = (m[0][2] + m[2][0]) / s;
	    this->q[3] = (m[2][1] - m[1][2]) / s;
	  }
	else if (max == 1)
	  {
	    const auto s = std::sqrt(1.0 - m[0][0] + m[1][1] - m[2][2]);
	    this->q[1] = 0.25 * s;
	    this->q[2] = (m[2][1] + m[1][2]) / s;
	    this->q[3] = (m[0][2] - m[2][0]) / s;
	    this->q[0] = (m[1][0] + m[0][1]) / s;
	  }
	else if (max == 2)
	  {
	    const auto s = std::sqrt(1.0 - m[0][0] - m[1][1] + m[2][2]);
	    this->q[2] = 0.25 * s;
	    this->q[3] = (m[1][0] - m[0][1]) / s;
	    this->q[0] = (m[0][2] + m[2][0]) / s;
	    this->q[1] = (m[2][1] + m[1][2]) / s;
	  }
      }
  }

/**
 *
 */
template<typename _Tp>
  void
  quaternion<_Tp>::get(_Tp Q[4]) const
  {
    Q[0] = this->q[0];
    Q[1] = this->q[1];
    Q[2] = this->q[2];
    Q[3] = this->q[3];

    return;
  }

/**
 *
 */
template<typename _Tp>
  void
  quaternion<_Tp>::get(_Tp& q0, _Tp& q1, _Tp& q2, _Tp& q3) const
  {
    q0 = this->q[0];
    q1 = this->q[1];
    q2 = this->q[2];
    q3 = this->q[3];
  }

/**
 *
 */
template<typename _Tp>
  void
  quaternion<_Tp>::get(_Tp& w, _Tp Q[3]) const
  {
    w = this->q[0];
    Q[0] = this->q[1];
    Q[1] = this->q[2];
    Q[2] = this->q[3];
  }

/**
 *  Get the quaternion<_Tp> into a _Tp 3x3 orthogonal transformation matrix.
 *  Return the overall transform scale factor.
 */
template<typename _Tp>
  _Tp
  quaternion<_Tp>::get(_Tp m[3][3]) const
  {
    const auto ww = this->q[0] * this->q[0], wx = this->q[0] * this->q[1], wy = this->q[0] * this->q[2], wz = this->q[0] * this->q[3];
    const auto xx = this->q[1] * this->q[1], xy = this->q[1] * this->q[2], xz = this->q[1] * this->q[3];
    const auto yy = this->q[2] * this->q[2], yz = this->q[2] * this->q[3];
    const auto zz = this->q[3] * this->q[3];
    const auto ll = ww + xx + yy + zz;

    m[0][0] = (ww + xx - yy - zz) / ll;
    m[0][1] = 2.0 * (xy - wz) / ll;
    m[0][2] = 2.0 * (xz + wy) / ll;

    m[1][0] = 2.0 * (xy + wz) / ll;
    m[1][1] = (ww - xx + yy - zz) / ll;
    m[1][2] = 2.0 * (yz - wx) / ll;

    m[2][0] = 2.0 * (xz - wy) / ll;
    m[2][1] = 2.0 * (yz + wx) / ll;
    m[2][2] = (ww - xx - yy + zz) / ll;

    return ll;
  }

/**
 *  Get the quaternion<_Tp> into a _Tp 4x4 transform matrix of the form
 *
 *  | Mxx  Mxy  Mxz  0 |
 *  | Myx  Myy  Myz  0 |
 *  | Mzx  Mzy  Mzz  0 |
 *  |  0    0    0   W |
 */
template<typename _Tp>
  void
  quaternion<_Tp>::get(_Tp m[4][4]) const
  {
    const auto  ww = this->q[0] * this->q[0], wx = this->q[0] * this->q[1], wy = this->q[0] * this->q[2], wz = this->q[0] * this->q[3];
    const auto  xx = this->q[1] * this->q[1], xy = this->q[1] * this->q[2], xz = this->q[1] * this->q[3];
    const auto  yy = this->q[2] * this->q[2], yz = this->q[2] * this->q[3];
    const auto  zz = this->q[3] * this->q[3];

    m[0][0] = ww + xx - yy - zz;
    m[0][1] = 2.0 * (xy - wz);
    m[0][2] = 2.0 * (xz + wy);

    m[1][0] = 2.0 * (xy + wz);
    m[1][1] = ww - xx + yy - zz;
    m[1][2] = 2.0 * (yz - wx);

    m[2][0] = 2.0 * (xz - wy);
    m[2][2] = 2.0 * (yz + wx);
    m[2][3] = ww - xx - yy + zz;

    m[3][0] = m[0][3] = 0.0;
    m[3][1] = m[1][3] = 0.0;
    m[3][2] = m[2][3] = 0.0;
    m[3][3] = ww + xx + yy + zz;
  }

/**
 *  Gets the quaternion<_Tp> in the form Q = radius.exp([ 0, axis.angle ])
 *  Where radius and angle are scalars and axis is a unit vector.
 */
template<typename _Tp>
  void
  quaternion<_Tp>::get(_Tp axis[3], _Tp& angle, _Tp& radius) const
  {
    radius = abs(*this);
    if (radius < 1.0e-16)
      {
	angle = 0.0;
	axis[0] = 0.0;
	axis[1] = 0.0;
	axis[2] = 1.0;
	return;
      }

    const auto cosangle = this->q[0] / radius;
    const auto sinangle = 1.0 - cosangle * cosangle;
    if (sinangle < 1.0e-16)
      {
	angle = 0.0;
	axis[0] = 0.0;
	axis[1] = 0.0;
	axis[2] = 1.0;
	return;
      }
    sinangle = std::sqrt(sinangle);

    angle = 2.0 * std::atan2(sinangle, cosangle);

    axis[0] = this->q[1] / (radius * sinangle);
    axis[1] = this->q[2] / (radius * sinangle);
    axis[2] = this->q[3] / (radius * sinangle);
  }

/**
 *
 */
template<typename _Tp>
  _Tp&
  quaternion<_Tp>::operator[](std::size_t i)
  { return this->q[i]; }

/**
 *
 */
template<typename _Tp>
  _Tp
  quaternion<_Tp>::operator[](std::size_t i) const
  { return this->q[i]; }

/**
 *
 */
template<typename _Tp>
  _Tp
  quaternion<_Tp>::w() const
  { return this->q[0]; }

/**
 *
 */
template<typename _Tp>
  void
  quaternion<_Tp>::w(_Tp w)
  { this->q[0] = w; }

/**
 *
 */
template<typename _Tp>
  _Tp
  quaternion<_Tp>::x() const
  { return this->q[1]; }

/**
 *
 */
template<typename _Tp>
  void
  quaternion<_Tp>::x(_Tp x)
  { this->q[1] = x; }

/**
 *
 */
template<typename _Tp>
  _Tp
  quaternion<_Tp>::y() const
  { return this->q[2]; }

/**
 *
 */
template<typename _Tp>
  void
  quaternion<_Tp>::y(_Tp y)
  { this->q[2] = y; }

/**
 *
 */
template<typename _Tp>
  _Tp
  quaternion<_Tp>::z() const
  { return this->q[3]; }

/**
 *
 */
template<typename _Tp>
  void
  quaternion<_Tp>::z(_Tp z)
  { this->q[3] = z; }

/**
 *  Add the input quaternion to this quaternion.
 *  Return the reference to this quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::add(const quaternion<_Tp>& Q)
  {
    this->q[0] += Q[0];
    this->q[1] += Q[1];
    this->q[2] += Q[2];
    this->q[3] += Q[3];

    return *this;
  }

/**
 *  Subtract the input quaternion to this quaternion.
 *  Return the reference to this quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::subtract(const quaternion<_Tp>& Q)
  {
    this->q[0] -= Q[0];
    this->q[1] -= Q[1];
    this->q[2] -= Q[2];
    this->q[3] -= Q[3];

    return *this;
  }

/**
 *  Multiply this quaternion by the input _Tp scalar.
 *  Return the reference to this quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::multiply(_Tp c)
  {
    this->q[0] *= c;
    this->q[1] *= c;
    this->q[2] *= c;
    this->q[3] *= c;

    return *this;
  }

/**
 *  Left multiply this quaternion by the input quaternion<_Tp>.
 *  Return the reference to this quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::lmultiply(const quaternion<_Tp>& Q)
  {
    quaternion<_Tp>  R;

    R[0] = Q[0] * this->q[0] - Q[1] * this->q[1] - Q[2] * this->q[2] - Q[3] * this->q[3];
    R[1] = Q[2] * this->q[3] - Q[3] * this->q[2] + Q[0] * this->q[1] + Q[1] * this->q[0];
    R[2] = Q[3] * this->q[1] - Q[1] * this->q[3] + Q[0] * this->q[2] + Q[2] * this->q[0];
    R[3] = Q[1] * this->q[2] - Q[2] * this->q[1] + Q[0] * this->q[3] + Q[3] * this->q[0];

    *this = R;

    return *this;
  }

/**
 *  Right multiply this quaternion by the input quaternion<_Tp>.
 *  Return the reference to this quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::rmultiply(const quaternion<_Tp>& Q)
  {
    quaternion<_Tp>  R;

    R[0] = this->q[0] * Q[0] - this->q[1] * Q[1] - this->q[2] * Q[2] - this->q[3] * Q[3];
    R[1] = this->q[2] * Q[3] - this->q[3] * Q[2] + this->q[0] * Q[1] + this->q[1] * Q[0];
    R[2] = this->q[3] * Q[1] - this->q[1] * Q[3] + this->q[0] * Q[2] + this->q[2] * Q[0];
    R[3] = this->q[1] * Q[2] - this->q[2] * Q[1] + this->q[0] * Q[3] + this->q[3] * Q[0];

    *this = R;

    return *this;
  }

/**
 *  Return the conjugate of the quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::conjugate()
  {
    this->q[1] = -q[1];
    this->q[2] = -q[2];
    this->q[3] = -q[3];

    return *this;
  }

/**
 *  Return the inverse of the quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::invert()
  {
    const auto n = norm(*this);

    this->q[0] = this->q[0] / n;
    this->q[1] = -q[1] / n;
    this->q[2] = -q[2] / n;
    this->q[3] = -q[3] / n;

    return *this;
  }

/**
 *  Return the sum of this quaternion with the input quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::sum(const quaternion<_Tp>& Q) const
  {
    quaternion<_Tp>  R;

    R[0] = this->q[0] + Q[0];
    R[1] = this->q[1] + Q[1];
    R[2] = this->q[2] + Q[2];
    R[3] = this->q[3] + Q[3];

    return R;
  }

/**
 *  Return the difference of this quaternion with the input quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::diff(const quaternion<_Tp>& Q) const
  {
    quaternion<_Tp>  R;

    R[0] = this->q[0] - Q[0];
    R[1] = this->q[1] - Q[1];
    R[2] = this->q[2] - Q[2];
    R[3] = this->q[3] - Q[3];

    return R;
  }

/**
 *  Return the left product of the input quaternion<_Tp> with the quaternion.
 *  The return value is R = Qq where q is the current Quaterion and Q is input.
 *  Multiplication is NOT commutative: Qq != qQ.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::lprod(const quaternion<_Tp>& Q) const
  {
    quaternion<_Tp> R;

    R[0] = Q[0] * this->q[0] - Q[1] * this->q[1] - Q[2] * this->q[2] - Q[3] * this->q[3];
    R[1] = Q[2] * this->q[3] - Q[3] * this->q[2] + Q[0] * this->q[1] + Q[1] * this->q[0];
    R[2] = Q[3] * this->q[1] - Q[1] * this->q[3] + Q[0] * this->q[2] + Q[2] * this->q[0];
    R[3] = Q[1] * this->q[2] - Q[2] * this->q[1] + Q[0] * this->q[3] + Q[3] * this->q[0];

    return R;
  }

/**
 *  Return the right product of the quaternion with the input quaternion<_Tp>.
 *  The return value is R = qQ where q is the current Quaterion and Q is input.
 *  Multiplication is NOT commutative: Qq != qQ.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::rprod(const quaternion<_Tp>& Q) const
  {
    quaternion<_Tp> R;

    R[0] = this->q[0] * Q[0] - this->q[1] * Q[1] - this->q[2] * Q[2] - this->q[3] * Q[3];
    R[1] = this->q[2] * Q[3] - this->q[3] * Q[2] + this->q[0] * Q[1] + this->q[1] * Q[0];
    R[2] = this->q[3] * Q[1] - this->q[1] * Q[3] + this->q[0] * Q[2] + this->q[2] * Q[0];
    R[3] = this->q[1] * Q[2] - this->q[2] * Q[1] + this->q[0] * Q[3] + this->q[3] * Q[0];

    return R;
  }

/**
 *  Return the product of the quaternion<_Tp> with the input scalar.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::prod(_Tp c) const
  {
    quaternion<_Tp> R;

    R[0] = c * this->q[0];
    R[1] = c * this->q[1];
    R[2] = c * this->q[2];
    R[3] = c * this->q[3];

    return R;
  }

/**
 *  Return the quotient of the quaternion<_Tp> with the input scalar.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::quot(_Tp c) const
  {
    quaternion<_Tp> R;

    R[0] = this->q[0] / c;
    R[1] = this->q[1] / c;
    R[2] = this->q[2] / c;
    R[3] = this->q[3] / c;

    return R;
  }

/**
 *  Return the conjugate of the quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>
  conj(const quaternion<_Tp>& Q)
  { return quaternion<_Tp>(Q[0], -Q[1], -Q[2], -Q[3]); }

/**
 *  Return the inverse of the quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>
  inv(const quaternion<_Tp>& Q)
  {
    const auto n = norm(Q);

    quaternion<_Tp> R;
    R[0] = Q[0] / n;
    R[1] = -Q[1] / n;
    R[2] = -Q[2] / n;
    R[3] = -Q[3] / n;

    return R;
  }

/**
 *  Return the squared length of the quaternion<_Tp>.
 */
template<typename _Tp>
  _Tp
  norm(const quaternion<_Tp>& Q)
  { return Q[0] * Q[0] + Q[1] * Q[1] + Q[2] * Q[2] + Q[3] * Q[3]; }

/**
 *  Return the length of the quaternion<_Tp>.
 */
template<typename _Tp>
  _Tp
  abs(const quaternion<_Tp>& Q)
  { return std::sqrt(Q[0] * Q[0] + Q[1] * Q[1] + Q[2] * Q[2] + Q[3] * Q[3]); }

/**
 *  Return the scalar product P.Q of two quaternions.
 */
template<typename _Tp>
  _Tp
  scalprod(const quaternion<_Tp> &P, const quaternion<_Tp>& Q)
  { return P[0] * Q[0] + P[1] * Q[1] + P[2] * Q[2] + P[3] * Q[3]; }

/**
 *  Rotate the input _Tp vector with this quaternion<_Tp>.
 */
template<typename _Tp>
  void
  rotate(const quaternion<_Tp>& Q, _Tp vector[3])
  {
    quaternion<_Tp> V(0.0, vector);
    quaternion<_Tp> R = V.rprod(inv(Q));
    R = R.lprod(Q);

    vector[0] = V[1];
    vector[1] = V[2];
    vector[2] = V[3];

    return;
  }

/**
 *  Return this quaternion<_Tp> renormalized to a length of c.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::renormalize(_Tp c)
  {
    const auto a = abs(*this);
    if (a < 1.0e-16)
      return *this;

    this->multiply(c / a);

    return *this;
  }

/**
 *  Returns the exponent of the given quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>
  exp(const quaternion<_Tp>& Q)
  {
    const auto l = std::sqrt(Q[1] * Q[1] + Q[2] * Q[2] + Q[3] * Q[3]);
    if (l < 1.0e-16)
      return quaternion<_Tp>(std::exp(Q[0]));

    const auto r = std::exp(Q[0]);
    const auto lfact = r * std::sin(l) / l;
    quaternion<_Tp> R;
    R[0] = r * std::cos(l);
    R[1] = Q[1] * lfact;
    R[2] = Q[2] * lfact;
    R[3] = Q[3] * lfact;

    return R;
  }

/**
 *  Returns the log of the given quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>
  log(const quaternion<_Tp>& Q)
  {
    const _Tp axis[3], angle, radius;
    Q.get(axis, angle, radius);

    return quaternion<_Tp>(std::log(radius), angle * axis[0], angle * axis[1], angle * axis[2]);
  }

/**
 *  Raise the input quaternion<_Tp> to the input power.
 */
template<typename _Tp>
  quaternion<_Tp>
  pow(const quaternion<_Tp>& Q, _Tp x)
  {
    const _Tp axis[3], angle, radius;
    Q.get(axis, angle, radius);

    radius = std::pow(radius, x);
    angle *= x;

    quaternion<_Tp> R;
    const auto sangle = radius * std::sin(0.5 * angle);
    R[0] = radius * std::cos(0.5 * angle);
    R[1] = axis[0] * sangle;
    R[2] = axis[1] * sangle;
    R[3] = axis[2] * sangle;

    return R;
  }

/**
 *  Return the linear interpolation of this quaternion qith the input quaternion
 *  by the input fraction t.
 */
template<typename _Tp>
  quaternion<_Tp>
  slerp(const quaternion<_Tp>& P, const quaternion<_Tp>& Q, _Tp t)
  {
    quaternion<_Tp> R = pow(Q.rprod(inv(P)), t);

    return R *= P;
  }

/**
 *
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::operator*=(_Tp c)
  {
    this->multiply(c);

    return *this;
  }

/**
 *  Right multiply this quaternion<_Tp> by the right quaternion<_Tp> and return
 *  a reference to this quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::operator*=(const quaternion<_Tp>& Q)
  {
    this->rmultiply(Q);

    return *this;
  }

/**
 *
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::operator/=(_Tp c)
  {
    if (c == 0.0)
      {
	std::cerr << "\n  Divide by zero error in quaternion<_Tp>.\n";
	return *this;
      }

    this->multiply(1.0 / c);

    return *this;
  }

/**
 *  Divide this quaternion<_Tp> by the right quaternion<_Tp> and return
 *  a reference to this quaternion<_Tp>.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::operator/=(const quaternion<_Tp>& Q)
  {
    this->rmultiply(inv(Q));

    return *this;
  }

/**
 *  Unary plus.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::operator+() const
  { return *this; }

/**
 *  Unary minus.
 */
template<typename _Tp>
  quaternion<_Tp>
  quaternion<_Tp>::operator-() const
  { return quaternion<_Tp>(*this).multiply(-1.0); }

/**
 *  Assign a scalar to this quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::operator=(_Tp q0)
  {
    this->set(q0);

    return *this;
  }

/**
 *  Add the input quaternion to this.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::operator+=(const quaternion<_Tp>& Q)
  {
    this->q[0] += Q[0];
    this->q[1] += Q[1];
    this->q[2] += Q[2];
    this->q[3] += Q[3];

    return *this;
  }

/**
 *  Subtract the input quaternion to this.
 */
template<typename _Tp>
  quaternion<_Tp>&
  quaternion<_Tp>::operator-=(const quaternion<_Tp>& Q)
  {
    this->q[0] -= Q[0];
    this->q[1] -= Q[1];
    this->q[2] -= Q[2];
    this->q[3] -= Q[3];

    return *this;
  }

/**
 *  Non-member function returning the sum of two quaternions.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator+(const quaternion<_Tp>& P, const quaternion<_Tp>& Q)
  { return P.sum(Q); }

/**
 *  Non-member function returning the difference of two quaternions.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator-(const quaternion<_Tp>& P, const quaternion<_Tp>& Q)
  { return P.diff(Q); }

/**
 *  Non-member function returning the product of a quaternion and a scalar.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator*(const quaternion<_Tp>& Q, _Tp c)
  { return Q.prod(c); }

/**
 *  Non-member function returning the product of two quaternions.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator*(const quaternion<_Tp>& P, const quaternion<_Tp>& Q)
  { return P.rprod(Q); }

/**
 *  Non-member function returning the product of a scalar and a quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator*(_Tp c, const quaternion<_Tp>& Q)
  { return Q.prod(c); }

/**
 *  Non-member function to divide a quaternion by a scalar and return
 *  a quaternion.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator/(const quaternion<_Tp>& Q, _Tp c)
  {
    if (c == 0.0)
      {
        std::cerr << "\n  Divide by zero error in quaternion<_Tp>.\n";
        return Q;
      }

    return Q.prod(1.0 / c);
  }

/**
 *  Non-member function returning the quotient of two quaternions.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator/(const quaternion<_Tp>& P, const quaternion<_Tp>& Q)
  { return P.rprod(inv(Q)); }

/**
 *  Non-member function to divide a scalar by a quaternion
 *  and return a quaterion.
 */
template<typename _Tp>
  quaternion<_Tp>
  operator/(_Tp c, const quaternion<_Tp>& Q)
  { return inv(Q).multiply(c); }

/**
 *  Non-member function returning true if two quaternions are equal.
 */
template<typename _Tp>
  bool
  operator==(const quaternion<_Tp>& P, const quaternion<_Tp>& Q)
  {
    return P[0] == Q[0]
        && P[1] == Q[1]
        && P[2] == Q[2]
        && P[3] == Q[3];
  }

/**
 *  Non-member function returning true if two quaternions are not equal.
 */
template<typename _Tp>
  bool
  operator!=(const quaternion<_Tp>& P, const quaternion<_Tp>& Q)
  { return !(P == Q); }

/**
 *  Input extraction for a quaternion.
 *  Accepted formats are:
 *    q0
 *    (q0)
 *    (q0,q1,q2,q3)
 */
template<typename CharT, typename Traits, typename _Tp>
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, quaternion<_Tp>& Q)
  {
    _Tp q[4] = {0.0, 0.0, 0.0, 0.0};
    CharT ch;
    is >> ch;
    if (ch == '(') 
      {
        is >> q[0] >> ch;
        if (ch == ',') 
          {
            for (std::size_t i = 1; i < 4 && ch == ','; ++i)
              is >> q[i] >> ch;
            if (ch == ')')
              Q = quaternion<_Tp>(q);
            else
              is.setstate(std::ios_base::failbit);
          }
        else if (ch == ')') 
          Q = quaternion<_Tp>(q);
        else
          is.setstate(std::ios_base::failbit);
      }
    else 
      {
        is.putback(ch);
        is >> q[0];
        Q = quaternion<_Tp>(q);
      }

    return is;
  }

/**
 *  Output inserter for a quaternion.
 *  The output format is (q0,q1,q2,q3)
 */
template<typename CharT, typename Traits, typename _Tp>
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const quaternion<_Tp>& Q)
  {
    std::basic_ostringstream<CharT, Traits> oss;
    oss.flags(os.flags());
    oss.imbue(os.getloc());
    oss.precision(os.precision());
    oss << '(' << Q[0] << ',' << Q[1] << ',' << Q[2] << ',' << Q[3] << ')';
    return os << oss.str();
  }


} // namespace std

#endif // QUATERNION_TCC
