#pragma once

#include "vector.hpp"
#include "utilities/logger.hpp"


namespace detail {

	/// \brief Use ADL only to create operators for all derived vector types.
	/// \details The implementation of the operators assume all assignment operators
	///		to be existent.
	///		
	///		The array access operators are using a trick: this class is known to
	///		be part of a class which only contains pure data. This is only valid
	///		in the current context! Inheriting in an other way (virtual/other
	///		members this would cause hard crashes.
	///		
	///		Unfortunately this class cannot be in a detail namespace because ADL of
	///		Operators would stop working
	template<typename Data>
	struct CommonVectorOperators
	{
		/// \brief Read/Write array access operator.
		Data& operator [] (int _index)
		{
			// If you crash here you used either wrong inheritance or you accessed
			// with a wrong index on a high level vector class.
			return *((Data*)this + _index);
		}

		/// \brief Read only array access operator.
		Data operator [] (int _index) const
		{ 
			// If you crash here you used either wrong inheritance or you accessed
			// with a wrong index on a high level vector class.
			return *((Data*)this + _index);
		}
	};

	// ************************************************************************* //
	/// \brief Type for "swizzled" access to single element vectors.
	/// \details There is no swizzling for a single element but this class is
	///		required for operator definition.
	template<typename VectorType, typename Data, int A>
	class SwizzleProxy1: public CommonVectorOperators<Data>
	{
	public:
		/// \brief To be a write able proxy all indices must be different
		static const bool IsWritable = true;

		/// \brief Use this type if an function should be created only if the
		///		current swizzle is write able.
		typedef typename std::conditional<IsWritable, SwizzleProxy1, struct OperationNotAvailable>::type WriteableThisType;

		/// \brief Auto cast to the elementary type.
		operator Data() const
		{
			return ((Data*)this)[A];
		}

		/// \brief Use a locally defined macro to reduce the vector
		///		implementation overhead.
#		define CREATE_ASSIGMENT_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2>								\
		WriteableThisType& operator Op (const SwizzleProxy1<VectorType2, Data2, A2>& _rhs)	\
		{																				\
			((Data*)this)[A] Op ((const Data2*)&_rhs)[A2];								\
			return *this;																\
		}																				\
																						\
		/* Scalar operation */															\
		WriteableThisType& operator Op (const Data _rhs)								\
		{																				\
			((Data*)this)[A] Op _rhs;													\
			return *this;																\
		}

		CREATE_ASSIGMENT_OPERATOR( = )

		CREATE_ASSIGMENT_OPERATOR( += )
		CREATE_ASSIGMENT_OPERATOR( -= )
		CREATE_ASSIGMENT_OPERATOR( *= )
		CREATE_ASSIGMENT_OPERATOR( /= )

		// The following operators are only defined for integer types.
		CREATE_ASSIGMENT_OPERATOR( |= )
		CREATE_ASSIGMENT_OPERATOR( &= )
		CREATE_ASSIGMENT_OPERATOR( ^= )
		CREATE_ASSIGMENT_OPERATOR( %= )
		CREATE_ASSIGMENT_OPERATOR( <<= )
		CREATE_ASSIGMENT_OPERATOR( >>= )

#		undef CREATE_ASSIGMENT_OPERATOR

#		define CREATE_ARITHMETIC_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2>								\
		Data operator Op (const SwizzleProxy1<VectorType2, Data2, A2>& _rhs) const		\
		{																				\
			return ((const Data*)this)[A] Op ((const Data2*)&_rhs)[A2];					\
		}																				\
		\
		Data operator Op (const Data _rhs) const										\
		{																				\
			return ((const Data*)this)[A] Op _rhs;										\
		}																				\
		\
		friend Data operator Op (const Data _lhs, const SwizzleProxy1& _rhs)			\
		{																				\
			return _lhs Op ((const Data*)&_rhs)[A];										\
		}

		CREATE_ARITHMETIC_OPERATOR(+)
		CREATE_ARITHMETIC_OPERATOR(-)
		CREATE_ARITHMETIC_OPERATOR(*)
		CREATE_ARITHMETIC_OPERATOR(/)

		// Integer only operators
		CREATE_ARITHMETIC_OPERATOR(|)
		CREATE_ARITHMETIC_OPERATOR(&)
		CREATE_ARITHMETIC_OPERATOR(^)
		CREATE_ARITHMETIC_OPERATOR(%)
		CREATE_ARITHMETIC_OPERATOR(<<)
		CREATE_ARITHMETIC_OPERATOR(>>)

#		undef CREATE_ARITHMETIC_OPERATOR
	};


	// ************************************************************************* //
	/// \brief Type for swizzled access to two element vectors.
	/// \details All the vector operators are defined on the swizzle types. The
	///		derived final vector classes are only unions of swizzle vectors.
	template<typename VectorType, typename Data, int A, int B>
	class SwizzleProxy2: public CommonVectorOperators<Data>
	{
	public:
		/// \brief To be a write able proxy all indices must be different
		static const bool IsWritable = (A != B);

		/// \brief Use this type if an function should be created only if the
		///		current swizzle is write able.
		typedef typename std::conditional<IsWritable, SwizzleProxy2, struct OperationNotAvailable>::type WriteableThisType;

		/// \brief Use a locally defined macro to reduce the vector
		///		implementation overhead.
#		define CREATE_ASSIGMENT_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2, int B2>						\
		WriteableThisType& operator Op (const SwizzleProxy2<VectorType2, Data2, A2, B2>& _rhs)	\
		{																				\
			((Data*)this)[A] Op ((const Data2*)&_rhs)[A2];								\
			((Data*)this)[B] Op ((const Data2*)&_rhs)[B2];								\
			return *this;																\
		}																				\
																						\
		/* Scalar operation */															\
		WriteableThisType& operator Op (const Data _rhs)								\
		{																				\
			((Data*)this)[A] Op _rhs;													\
			((Data*)this)[B] Op _rhs;													\
			return *this;																\
		}

		CREATE_ASSIGMENT_OPERATOR( = )

		CREATE_ASSIGMENT_OPERATOR( += )
		CREATE_ASSIGMENT_OPERATOR( -= )
		CREATE_ASSIGMENT_OPERATOR( *= )
		CREATE_ASSIGMENT_OPERATOR( /= )

		// The following operators are only defined for integer types.
		CREATE_ASSIGMENT_OPERATOR( |= )
		CREATE_ASSIGMENT_OPERATOR( &= )
		CREATE_ASSIGMENT_OPERATOR( ^= )
		CREATE_ASSIGMENT_OPERATOR( %= )
		CREATE_ASSIGMENT_OPERATOR( <<= )
		CREATE_ASSIGMENT_OPERATOR( >>= )

#		undef CREATE_ASSIGMENT_OPERATOR

#		define CREATE_ARITHMETIC_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2, int B2>						\
		VectorType operator Op (const SwizzleProxy2<VectorType2, Data2, A2, B2>& _rhs) const	\
		{																				\
			VectorType result;															\
			result[0] = ((const Data*)this)[A] Op ((const Data2*)&_rhs)[A2];			\
			result[1] = ((const Data*)this)[B] Op ((const Data2*)&_rhs)[B2];			\
			return result;																\
		}																				\
		\
		VectorType operator Op (const Data _rhs) const									\
		{																				\
			VectorType result;															\
			result[0] =	((const Data*)this)[A] Op _rhs;									\
			result[1] =	((const Data*)this)[B] Op _rhs;									\
			return result;																\
		}																				\
		\
		friend VectorType operator Op (const Data _lhs, const SwizzleProxy2& _rhs)		\
		{																				\
			VectorType result;															\
			result[0] = _lhs Op ((const Data*)&_rhs)[A];								\
			result[1] = _lhs Op ((const Data*)&_rhs)[B];								\
			return result;																\
		}

		CREATE_ARITHMETIC_OPERATOR(+)
		CREATE_ARITHMETIC_OPERATOR(-)
		CREATE_ARITHMETIC_OPERATOR(*)
		CREATE_ARITHMETIC_OPERATOR(/)

		// Integer only operators
		CREATE_ARITHMETIC_OPERATOR(|)
		CREATE_ARITHMETIC_OPERATOR(&)
		CREATE_ARITHMETIC_OPERATOR(^)
		CREATE_ARITHMETIC_OPERATOR(%)
		CREATE_ARITHMETIC_OPERATOR(<<)
		CREATE_ARITHMETIC_OPERATOR(>>)

#		undef CREATE_ARITHMETIC_OPERATOR
	};

	// ************************************************************************* //
	/// \brief Type for swizzled access to three element vectors.
	/// \details All the vector operators are defined on the swizzle types. The
	///		derived final vector classes are only unions of swizzle vectors.
	template<typename VectorType, typename Data, int A, int B, int C>
	class SwizzleProxy3: public CommonVectorOperators<Data>
	{
	public:
		/// \brief To be a write able proxy all indices must be different
		static const bool IsWritable = (A != B) && (A != C) && (B != C);

		/// \brief Use this type if an function should be created only if the
		///		current swizzle is write able.
		typedef typename std::conditional<IsWritable, SwizzleProxy3, struct OperationNotAvailable>::type WriteableThisType;

		/// \brief Use a locally defined macro to reduce the vector
		///		implementation overhead.
#		define CREATE_ASSIGMENT_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2, int B2, int C2>				\
		WriteableThisType& operator Op (const SwizzleProxy3<VectorType2, Data2, A2, B2, C2>& _rhs)	\
		{																				\
			((Data*)this)[A] Op ((const Data2*)&_rhs)[A2];								\
			((Data*)this)[B] Op ((const Data2*)&_rhs)[B2];								\
			((Data*)this)[C] Op ((const Data2*)&_rhs)[C2];								\
			return *this;																\
		}																				\
		\
		/* Scalar operation */															\
		WriteableThisType& operator Op (const Data _rhs)								\
		{																				\
			((Data*)this)[A] Op _rhs;													\
			((Data*)this)[B] Op _rhs;													\
			((Data*)this)[C] Op _rhs;													\
			return *this;																\
		}

		CREATE_ASSIGMENT_OPERATOR( = )

		CREATE_ASSIGMENT_OPERATOR( += )
		CREATE_ASSIGMENT_OPERATOR( -= )
		CREATE_ASSIGMENT_OPERATOR( *= )
		CREATE_ASSIGMENT_OPERATOR( /= )

		// The following operators are only defined for integer types.
		CREATE_ASSIGMENT_OPERATOR( |= )
		CREATE_ASSIGMENT_OPERATOR( &= )
		CREATE_ASSIGMENT_OPERATOR( ^= )
		CREATE_ASSIGMENT_OPERATOR( %= )
		CREATE_ASSIGMENT_OPERATOR( <<= )
		CREATE_ASSIGMENT_OPERATOR( >>= )

#		undef CREATE_ASSIGMENT_OPERATOR

#		define CREATE_ARITHMETIC_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2, int B2, int C2>				\
		VectorType operator Op (const SwizzleProxy3<VectorType2, Data2, A2, B2, C2>& _rhs) const	\
		{																				\
			VectorType result;															\
			result[0] = ((const Data*)this)[A] Op ((const Data2*)&_rhs)[A2];			\
			result[1] = ((const Data*)this)[B] Op ((const Data2*)&_rhs)[B2];			\
			result[2] = ((const Data*)this)[C] Op ((const Data2*)&_rhs)[C2];			\
			return result;																\
		}																				\
		\
		VectorType operator Op (const Data _rhs) const									\
		{																				\
			VectorType result;															\
			result[0] =	((const Data*)this)[A] Op _rhs;									\
			result[1] =	((const Data*)this)[B] Op _rhs;									\
			result[2] =	((const Data*)this)[C] Op _rhs;									\
			return result;																\
		}																				\
		\
		friend VectorType operator Op (const Data _lhs, const SwizzleProxy3& _rhs)		\
		{																				\
			VectorType result;															\
			result[0] = _lhs Op ((const Data*)&_rhs)[A];								\
			result[1] = _lhs Op ((const Data*)&_rhs)[B];								\
			result[2] = _lhs Op ((const Data*)&_rhs)[C];								\
			return result;																\
		}

		CREATE_ARITHMETIC_OPERATOR(+)
		CREATE_ARITHMETIC_OPERATOR(-)
		CREATE_ARITHMETIC_OPERATOR(*)
		CREATE_ARITHMETIC_OPERATOR(/)

		// Integer only operators
		CREATE_ARITHMETIC_OPERATOR(|)
		CREATE_ARITHMETIC_OPERATOR(&)
		CREATE_ARITHMETIC_OPERATOR(^)
		CREATE_ARITHMETIC_OPERATOR(%)
		CREATE_ARITHMETIC_OPERATOR(<<)
		CREATE_ARITHMETIC_OPERATOR(>>)

#		undef CREATE_ARITHMETIC_OPERATOR
	};

	// ************************************************************************* //
	/// \brief Type for swizzled access to four element vectors.
	/// \details All the vector operators are defined on the swizzle types. The
	///		derived final vector classes are only unions of swizzle vectors.
	template<typename VectorType, typename Data, int A, int B, int C, int D>
	class SwizzleProxy4: public CommonVectorOperators<Data>
	{
	public:
		/// \brief To be a write able proxy all indices must be different
		static const bool IsWritable = (A != B) && (A != C) && (A != D) && (B != C)
			&& (B != D) && (C != D);

		/// \brief Use this type if an function should be created only if the
		///		current swizzle is write able.
		typedef typename std::conditional<IsWritable, SwizzleProxy4, struct OperationNotAvailable>::type WriteableThisType;

		/// \brief Use a locally defined macro to reduce the vector
		///		implementation overhead.
#		define CREATE_ASSIGMENT_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2, int B2, int C2, int D2>		\
		WriteableThisType& operator Op (const SwizzleProxy4<VectorType2, Data2, A2, B2, C2, D2>& _rhs)	\
		{																				\
			((Data*)this)[A] Op ((const Data2*)&_rhs)[A2];								\
			((Data*)this)[B] Op ((const Data2*)&_rhs)[B2];								\
			((Data*)this)[C] Op ((const Data2*)&_rhs)[C2];								\
			((Data*)this)[D] Op ((const Data2*)&_rhs)[D2];								\
			return *this;																\
		}																				\
		\
		/* Scalar operation */															\
		WriteableThisType& operator Op (const Data _rhs)								\
		{																				\
			((Data*)this)[A] Op _rhs;													\
			((Data*)this)[B] Op _rhs;													\
			((Data*)this)[C] Op _rhs;													\
			((Data*)this)[D] Op _rhs;													\
			return *this;																\
		}

		CREATE_ASSIGMENT_OPERATOR( = )

		CREATE_ASSIGMENT_OPERATOR( += )
		CREATE_ASSIGMENT_OPERATOR( -= )
		CREATE_ASSIGMENT_OPERATOR( *= )
		CREATE_ASSIGMENT_OPERATOR( /= )

		// The following operators are only defined for integer types.
		CREATE_ASSIGMENT_OPERATOR( |= )
		CREATE_ASSIGMENT_OPERATOR( &= )
		CREATE_ASSIGMENT_OPERATOR( ^= )
		CREATE_ASSIGMENT_OPERATOR( %= )
		CREATE_ASSIGMENT_OPERATOR( <<= )
		CREATE_ASSIGMENT_OPERATOR( >>= )

#		undef CREATE_ASSIGMENT_OPERATOR

#		define CREATE_ARITHMETIC_OPERATOR(Op)											\
		template<class VectorType2, typename Data2, int A2, int B2, int C2, int D2>		\
		VectorType operator Op (const SwizzleProxy4<VectorType2, Data2, A2, B2, C2, D2>& _rhs) const	\
		{																				\
			VectorType result;															\
			result[0] = ((const Data*)this)[A] Op ((const Data2*)&_rhs)[A2];			\
			result[1] = ((const Data*)this)[B] Op ((const Data2*)&_rhs)[B2];			\
			result[2] = ((const Data*)this)[C] Op ((const Data2*)&_rhs)[C2];			\
			result[3] = ((const Data*)this)[D] Op ((const Data2*)&_rhs)[D2];			\
			return result;																\
		}																				\
		\
		VectorType operator Op (const Data _rhs) const									\
		{																				\
			VectorType result;															\
			result[0] =	((const Data*)this)[A] Op _rhs;									\
			result[1] =	((const Data*)this)[B] Op _rhs;									\
			result[2] =	((const Data*)this)[C] Op _rhs;									\
			result[3] =	((const Data*)this)[D] Op _rhs;									\
			return result;																\
		}																				\
		\
		friend VectorType operator Op (const Data _lhs, const SwizzleProxy4& _rhs)		\
		{																				\
			VectorType result;															\
			result[0] = _lhs Op ((const Data*)&_rhs)[A];								\
			result[1] = _lhs Op ((const Data*)&_rhs)[B];								\
			result[2] = _lhs Op ((const Data*)&_rhs)[C];								\
			result[3] = _lhs Op ((const Data*)&_rhs)[D];								\
			return result;																\
		}

		CREATE_ARITHMETIC_OPERATOR(+)
		CREATE_ARITHMETIC_OPERATOR(-)
		CREATE_ARITHMETIC_OPERATOR(*)
		CREATE_ARITHMETIC_OPERATOR(/)

		// Integer only operators
		CREATE_ARITHMETIC_OPERATOR(|)
		CREATE_ARITHMETIC_OPERATOR(&)
		CREATE_ARITHMETIC_OPERATOR(^)
		CREATE_ARITHMETIC_OPERATOR(%)
		CREATE_ARITHMETIC_OPERATOR(<<)
		CREATE_ARITHMETIC_OPERATOR(>>)

#		undef CREATE_ARITHMETIC_OPERATOR
	};

} // namespace detail

template<typename Data> struct Vec1_Base;
template<typename Data> struct Vec2_Base;
template<typename Data> struct Vec3_Base;
template<typename Data> struct Vec4_Base;

/// \brief Implementation of a 1D vector class with swizzling.
/// \details This allows to use scalars with swizzle access v.xxx.
template<typename Data>
struct Vec1_Base: public detail::SwizzleProxy1<Vec1_Base<Data>,Data,0>
{
	/// \brief The data with a multitude of access functions
	union {
		float m_data[1];

		detail::SwizzleProxy1<Vec1_Base<Data>,Data,0> x, r;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,0> xx, rr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,0> xxx, rrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,0> xxxx, rrrr;
	};

	/// \brief Fast default construction without initialization
	Vec1_Base() {}
	/// \brief Implicit construction from scalar!
	Vec1_Base(Data _x) { m_data[0] = _x; }
};


/// \brief Implementation of a 2D vector class with swizzling.
template<typename Data>
struct Vec2_Base: public detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,1>
{
	typedef Data DataType;

	/// \brief The data with a multitude of access functions
	union {
		DataType m_data[2];

		detail::SwizzleProxy1<Vec1_Base<Data>,Data,0> x, r;
		detail::SwizzleProxy1<Vec1_Base<Data>,Data,1> y, g;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,0> xx, rr;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,1> xy, rg;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,0> yx, gr;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,1> yy, gg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,0> xxx, rrr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,1> xxy, rrg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,0> xyx, rgr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,1> xyy, rgg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,0> yxx, grr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,1> yxy, grg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,0> yyx, ggr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,1> yyy, ggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,0> xxxx, rrrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,1> xxxy, rrrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,0> xxyx, rrgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,1> xxyy, rrgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,0> xyxx, rgrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,1> xyxy, rgrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,0> xyyx, rggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,1> xyyy, rggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,0> yxxx, grrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,1> yxxy, grrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,0> yxyx, grgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,1> yxyy, grgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,0> yyxx, ggrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,1> yyxy, ggrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,0> yyyx, gggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,1> yyyy, gggg;
	};

	/// \brief Fast default construction without initialization
	Vec2_Base() {}
	/// \brief Construction from scalar
	explicit Vec2_Base(Data _x)		{ m_data[0] = _x; m_data[1] = _x; }
	/// \brief Construction from two elements
	Vec2_Base(Data _x, Data _y)		{ m_data[0] = _x; m_data[1] = _y; }
	/// \brief creation from swizzle type
	template<typename VectorType, typename Data2, int A, int B>
	Vec2_Base( const detail::SwizzleProxy2<VectorType,Data2,A,B>& _v ) { m_data[0] = _v[A]; m_data[1] = _v[B]; }
	// Standard copy and assignment operator are defined as well
};


/// \brief Implementation of a 3D vector class with swizzling.
template<typename Data>
struct Vec3_Base: public detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,2>
{
	typedef Data DataType;

	/// \brief The data with a multitude of access functions
	union {
		DataType m_data[3];

		detail::SwizzleProxy1<Vec1_Base<Data>,Data,0> x, r;
		detail::SwizzleProxy1<Vec1_Base<Data>,Data,1> y, g;
		detail::SwizzleProxy1<Vec1_Base<Data>,Data,2> z, b;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,0> xx, rr;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,1> xy, rg;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,2> xz, rb;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,0> yx, gr;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,1> yy, gg;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,2> yz, gb;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,2,0> zx, br;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,2,1> zy, bg;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,2,2> zz, bb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,0> xxx, rrr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,1> xxy, rrg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,2> xxz, rrb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,0> xyx, rgr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,1> xyy, rgg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,2> xyz, rgb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,2,0> xzx, rbr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,2,1> xzy, rbg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,2,2> xzz, rbb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,0> yxx, grr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,1> yxy, grg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,2> yxz, grb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,0> yyx, ggr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,1> yyy, ggg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,2> yyz, ggb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,2,0> yzx, gbr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,2,1> yzy, gbg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,2,2> yzz, gbb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,0,0> zxx, brr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,0,1> zxy, brg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,0,2> zxz, brb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,1,0> zyx, bgr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,1,1> zyy, bgg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,1,2> zyz, bgb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,2,0> zzx, bbr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,2,1> zzy, bbg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,2,2> zzz, bbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,0> xxxx, rrrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,1> xxxy, rrrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,2> xxxz, rrrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,0> xxyx, rrgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,1> xxyy, rrgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,2> xxyz, rrgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,2,0> xxzx, rrbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,2,1> xxzy, rrbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,2,2> xxzz, rrbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,0> xyxx, rgrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,1> xyxy, rgrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,2> xyxz, rgrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,0> xyyx, rggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,1> xyyy, rggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,2> xyyz, rggb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,0> xyzx, rgbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,1> xyzy, rgbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,2> xyzz, rgbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,0,0> xzxx, rbrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,0,1> xzxy, rbrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,0,2> xzxz, rbrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,1,0> xzyx, rbgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,1,1> xzyy, rbgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,1,2> xzyz, rbgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,2,0> xzzx, rbbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,2,1> xzzy, rbbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,2,2> xzzz, rbbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,0> yxxx, grrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,1> yxxy, grrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,2> yxxz, grrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,0> yxyx, grgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,1> yxyy, grgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,2> yxyz, grgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,2,0> yxzx, grbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,2,1> yxzy, grbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,2,2> yxzz, grbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,0> yyxx, ggrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,1> yyxy, ggrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,2> yyxz, ggrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,0> yyyx, gggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,1> yyyy, gggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,2> yyyz, gggb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,2,0> yyzx, ggbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,2,1> yyzy, ggbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,2,2> yyzz, ggbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,0,0> yzxx, gbrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,0,1> yzxy, gbrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,0,2> yzxz, gbrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,1,0> yzyx, gbgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,1,1> yzyy, gbgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,1,2> yzyz, gbgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,2,0> yzzx, gbbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,2,1> yzzy, gbbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,2,2> yzzz, gbbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,0,0> zxxx, brrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,0,1> zxxy, brrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,0,2> zxxz, brrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,1,0> zxyx, brgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,1,1> zxyy, brgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,1,2> zxyz, brgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,2,0> zxzx, brbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,2,1> zxzy, brbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,2,2> zxzz, brbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,0,0> zyxx, bgrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,0,1> zyxy, bgrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,0,2> zyxz, bgrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,1,0> zyyx, bggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,1,1> zyyy, bggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,1,2> zyyz, bggb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,2,0> zyzx, bgbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,2,1> zyzy, bgbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,2,2> zyzz, bgbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,0,0> zzxx, bbrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,0,1> zzxy, bbrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,0,2> zzxz, bbrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,1,0> zzyx, bbgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,1,1> zzyy, bbgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,1,2> zzyz, bbgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,2,0> zzzx, bbbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,2,1> zzzy, bbbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,2,2> zzzz, bbbb;
	};

	/// \brief Fast default construction without initialization
	Vec3_Base() {}
	/// \brief Construction from scalar
	explicit Vec3_Base(Data _x)				{ m_data[0] = _x; m_data[1] = _x; m_data[2] = _x; }
	/// \brief Construction from two elements
	Vec3_Base(Data _x, Data _y, Data _z)	{ m_data[0] = _x; m_data[1] = _y; m_data[2] = _z; }
	/// \brief creation from swizzle type
	template<typename VectorType, typename Data2, int A, int B, int C>
	Vec3_Base( const detail::SwizzleProxy3<VectorType,Data2,A,B,C>& _v ) { m_data[0] = _v[A]; m_data[1] = _v[B]; m_data[2] = _v[C]; }
	// Standard copy and assignment operator are defined as well
};


/// \brief Implementation of a 4D vector class with swizzling.
template<typename Data>
struct Vec4_Base: public detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,3>
{
	typedef Data DataType;

	/// \brief The data with a multitude of access functions
	union {
		DataType m_data[4];

		detail::SwizzleProxy1<Vec1_Base<Data>,Data,0> x, r;
		detail::SwizzleProxy1<Vec1_Base<Data>,Data,1> y, g;
		detail::SwizzleProxy1<Vec1_Base<Data>,Data,2> z, b;
		detail::SwizzleProxy1<Vec1_Base<Data>,Data,3> w, a;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,0> xx, rr;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,1> xy, rg;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,2> xz, rb;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,0,3> xw, ra;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,0> yx, gr;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,1> yy, gg;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,2> yz, gb;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,1,3> yw, ga;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,2,0> zx, br;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,2,1> zy, bg;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,2,2> zz, bb;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,2,3> zw, ba;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,3,0> wx, ar;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,3,1> wy, ag;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,3,2> wz, ab;
		detail::SwizzleProxy2<Vec2_Base<Data>,Data,3,3> ww, aa;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,0> xxx, rrr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,1> xxy, rrg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,2> xxz, rrb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,0,3> xxw, rra;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,0> xyx, rgr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,1> xyy, rgg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,2> xyz, rgb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,1,3> xyw, rga;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,2,0> xzx, rbr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,2,1> xzy, rbg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,2,2> xzz, rbb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,2,3> xzw, rba;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,3,0> xwx, rar;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,3,1> xwy, rag;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,3,2> xwz, rab;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,0,3,3> xww, raa;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,0> yxx, grr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,1> yxy, grg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,2> yxz, grb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,0,3> yxw, gra;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,0> yyx, ggr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,1> yyy, ggg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,2> yyz, ggb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,1,3> yyw, gga;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,2,0> yzx, gbr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,2,1> yzy, gbg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,2,2> yzz, gbb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,2,3> yzw, gba;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,3,0> ywx, gar;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,3,1> ywy, gag;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,3,2> ywz, gab;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,1,3,3> yww, gaa;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,0,0> zxx, brr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,0,1> zxy, brg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,0,2> zxz, brb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,0,3> zxw, bra;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,1,0> zyx, bgr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,1,1> zyy, bgg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,1,2> zyz, bgb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,1,3> zyw, bga;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,2,0> zzx, bbr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,2,1> zzy, bbg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,2,2> zzz, bbb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,2,3> zzw, bba;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,3,0> zwx, bar;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,3,1> zwy, bag;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,3,2> zwz, bab;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,2,3,3> zww, baa;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,0,0> wxx, arr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,0,1> wxy, arg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,0,2> wxz, arb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,0,3> wxw, ara;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,1,0> wyx, agr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,1,1> wyy, agg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,1,2> wyz, agb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,1,3> wyw, aga;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,2,0> wzx, abr;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,2,1> wzy, abg;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,2,2> wzz, abb;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,2,3> wzw, aba;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,3,0> wwx, aar;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,3,1> wwy, aag;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,3,2> wwz, aab;
		detail::SwizzleProxy3<Vec3_Base<Data>,Data,3,3,3> www, aaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,0> xxxx, rrrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,1> xxxy, rrrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,2> xxxz, rrrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,0,3> xxxw, rrra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,0> xxyx, rrgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,1> xxyy, rrgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,2> xxyz, rrgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,1,3> xxyw, rrga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,2,0> xxzx, rrbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,2,1> xxzy, rrbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,2,2> xxzz, rrbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,2,3> xxzw, rrba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,3,0> xxwx, rrar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,3,1> xxwy, rrag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,3,2> xxwz, rrab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,0,3,3> xxww, rraa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,0> xyxx, rgrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,1> xyxy, rgrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,2> xyxz, rgrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,0,3> xyxw, rgra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,0> xyyx, rggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,1> xyyy, rggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,2> xyyz, rggb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,1,3> xyyw, rgga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,0> xyzx, rgbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,1> xyzy, rgbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,2> xyzz, rgbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,2,3> xyzw, rgba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,3,0> xywx, rgar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,3,1> xywy, rgag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,3,2> xywz, rgab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,1,3,3> xyww, rgaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,0,0> xzxx, rbrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,0,1> xzxy, rbrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,0,2> xzxz, rbrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,0,3> xzxw, rbra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,1,0> xzyx, rbgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,1,1> xzyy, rbgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,1,2> xzyz, rbgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,1,3> xzyw, rbga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,2,0> xzzx, rbbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,2,1> xzzy, rbbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,2,2> xzzz, rbbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,2,3> xzzw, rbba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,3,0> xzwx, rbar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,3,1> xzwy, rbag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,3,2> xzwz, rbab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,2,3,3> xzww, rbaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,0,0> xwxx, rarr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,0,1> xwxy, rarg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,0,2> xwxz, rarb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,0,3> xwxw, rara;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,1,0> xwyx, ragr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,1,1> xwyy, ragg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,1,2> xwyz, ragb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,1,3> xwyw, raga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,2,0> xwzx, rabr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,2,1> xwzy, rabg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,2,2> xwzz, rabb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,2,3> xwzw, raba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,3,0> xwwx, raar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,3,1> xwwy, raag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,3,2> xwwz, raab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,0,3,3,3> xwww, raaa;

		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,0> yxxx, grrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,1> yxxy, grrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,2> yxxz, grrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,0,3> yxxw, grra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,0> yxyx, grgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,1> yxyy, grgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,2> yxyz, grgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,1,3> yxyw, grga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,2,0> yxzx, grbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,2,1> yxzy, grbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,2,2> yxzz, grbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,2,3> yxzw, grba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,3,0> yxwx, grar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,3,1> yxwy, grag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,3,2> yxwz, grab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,0,3,3> yxww, graa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,0> yyxx, ggrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,1> yyxy, ggrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,2> yyxz, ggrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,0,3> yyxw, ggra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,0> yyyx, gggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,1> yyyy, gggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,2> yyyz, gggb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,1,3> yyyw, ggga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,2,0> yyzx, ggbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,2,1> yyzy, ggbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,2,2> yyzz, ggbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,2,3> yyzw, ggba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,3,0> yywx, ggar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,3,1> yywy, ggag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,3,2> yywz, ggab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,1,3,3> yyww, ggaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,0,0> yzxx, gbrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,0,1> yzxy, gbrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,0,2> yzxz, gbrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,0,3> yzxw, gbra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,1,0> yzyx, gbgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,1,1> yzyy, gbgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,1,2> yzyz, gbgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,1,3> yzyw, gbga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,2,0> yzzx, gbbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,2,1> yzzy, gbbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,2,2> yzzz, gbbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,2,3> yzzw, gbba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,3,0> yzwx, gbar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,3,1> yzwy, gbag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,3,2> yzwz, gbab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,2,3,3> yzww, gbaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,0,0> ywxx, garr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,0,1> ywxy, garg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,0,2> ywxz, garb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,0,3> ywxw, gara;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,1,0> ywyx, gagr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,1,1> ywyy, gagg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,1,2> ywyz, gagb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,1,3> ywyw, gaga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,2,0> ywzx, gabr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,2,1> ywzy, gabg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,2,2> ywzz, gabb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,2,3> ywzw, gaba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,3,0> ywwx, gaar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,3,1> ywwy, gaag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,3,2> ywwz, gaab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,1,3,3,3> ywww, gaaa;

		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,0,0> zxxx, brrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,0,1> zxxy, brrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,0,2> zxxz, brrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,0,3> zxxw, brra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,1,0> zxyx, brgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,1,1> zxyy, brgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,1,2> zxyz, brgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,1,3> zxyw, brga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,2,0> zxzx, brbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,2,1> zxzy, brbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,2,2> zxzz, brbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,2,3> zxzw, brba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,3,0> zxwx, brar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,3,1> zxwy, brag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,3,2> zxwz, brab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,0,3,3> zxww, braa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,0,0> zyxx, bgrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,0,1> zyxy, bgrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,0,2> zyxz, bgrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,0,3> zyxw, bgra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,1,0> zyyx, bggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,1,1> zyyy, bggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,1,2> zyyz, bggb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,1,3> zyyw, bgga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,2,0> zyzx, bgbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,2,1> zyzy, bgbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,2,2> zyzz, bgbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,2,3> zyzw, bgba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,3,0> zywx, bgar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,3,1> zywy, bgag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,3,2> zywz, bgab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,1,3,3> zyww, bgaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,0,0> zzxx, bbrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,0,1> zzxy, bbrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,0,2> zzxz, bbrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,0,3> zzxw, bbra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,1,0> zzyx, bbgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,1,1> zzyy, bbgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,1,2> zzyz, bbgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,1,3> zzyw, bbga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,2,0> zzzx, bbbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,2,1> zzzy, bbbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,2,2> zzzz, bbbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,2,3> zzzw, bbba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,3,0> zzwx, bbar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,3,1> zzwy, bbag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,3,2> zzwz, bbab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,2,3,3> zzww, bbaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,0,0> zwxx, barr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,0,1> zwxy, barg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,0,2> zwxz, barb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,0,3> zwxw, bara;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,1,0> zwyx, bagr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,1,1> zwyy, bagg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,1,2> zwyz, bagb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,1,3> zwyw, baga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,2,0> zwzx, babr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,2,1> zwzy, babg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,2,2> zwzz, babb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,2,3> zwzw, baba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,3,0> zwwx, baar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,3,1> zwwy, baag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,3,2> zwwz, baab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,2,3,3,3> zwww, baaa;

		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,0,0> wxxx, arrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,0,1> wxxy, arrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,0,2> wxxz, arrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,0,3> wxxw, arra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,1,0> wxyx, argr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,1,1> wxyy, argg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,1,2> wxyz, argb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,1,3> wxyw, arga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,2,0> wxzx, arbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,2,1> wxzy, arbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,2,2> wxzz, arbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,2,3> wxzw, arba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,3,0> wxwx, arar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,3,1> wxwy, arag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,3,2> wxwz, arab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,0,3,3> wxww, araa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,0,0> wyxx, agrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,0,1> wyxy, agrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,0,2> wyxz, agrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,0,3> wyxw, agra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,1,0> wyyx, aggr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,1,1> wyyy, aggg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,1,2> wyyz, aggb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,1,3> wyyw, agga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,2,0> wyzx, agbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,2,1> wyzy, agbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,2,2> wyzz, agbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,2,3> wyzw, agba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,3,0> wywx, agar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,3,1> wywy, agag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,3,2> wywz, agab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,1,3,3> wyww, agaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,0,0> wzxx, abrr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,0,1> wzxy, abrg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,0,2> wzxz, abrb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,0,3> wzxw, abra;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,1,0> wzyx, abgr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,1,1> wzyy, abgg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,1,2> wzyz, abgb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,1,3> wzyw, abga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,2,0> wzzx, abbr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,2,1> wzzy, abbg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,2,2> wzzz, abbb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,2,3> wzzw, abba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,3,0> wzwx, abar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,3,1> wzwy, abag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,3,2> wzwz, abab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,2,3,3> wzww, abaa;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,0,0> wwxx, aarr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,0,1> wwxy, aarg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,0,2> wwxz, aarb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,0,3> wwxw, aara;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,1,0> wwyx, aagr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,1,1> wwyy, aagg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,1,2> wwyz, aagb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,1,3> wwyw, aaga;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,2,0> wwzx, aabr;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,2,1> wwzy, aabg;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,2,2> wwzz, aabb;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,2,3> wwzw, aaba;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,3,0> wwwx, aaar;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,3,1> wwwy, aaag;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,3,2> wwwz, aaab;
		detail::SwizzleProxy4<Vec4_Base<Data>,Data,3,3,3,3> wwww, aaaa;
	};

	/// \brief Fast default construction without initialization
	Vec4_Base() {}
	/// \brief Construction from scalar
	explicit Vec4_Base(Data _x)						{ m_data[0] = _x; m_data[1] = _x; m_data[2] = _x; m_data[3] = _x; }
	/// \brief Construction from two elements
	Vec4_Base(Data _x, Data _y, Data _z, Data _w)	{ m_data[0] = _x; m_data[1] = _y; m_data[2] = _z; m_data[3] = _w; }
	/// \brief creation from swizzle type
	template<typename VectorType, typename Data2, int A, int B, int C, int D>
	Vec4_Base( const detail::SwizzleProxy4<VectorType,Data2,A,B,C,D>& _v ) { m_data[0] = _v[A]; m_data[1] = _v[B]; m_data[2] = _v[C]; m_data[3] = _v[D]; }
	// Standard copy and assignment operator are defined as well
};

static_assert(sizeof(Vec1_Base<float>) == sizeof(float[1]), "A vector must have exactly the size of its data.");
static_assert(sizeof(Vec2_Base<float>) == sizeof(float[2]), "A vector must have exactly the size of its data.");
static_assert(sizeof(Vec3_Base<float>) == sizeof(float[3]), "A vector must have exactly the size of its data.");
static_assert(sizeof(Vec4_Base<float>) == sizeof(float[4]), "A vector must have exactly the size of its data.");