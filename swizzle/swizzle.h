#include <type_traits>

template<typename VectorType, typename Data, int... I>
  class SwizzleProxy//: public CommonVectorOperators<Data>
  {
  public:
    /// To be a writeable proxy all indices must be different
    // I'd like a compile time tools for this for arb size!
    //static const bool IsWritable = (A != B);
    static const bool std::common_type<std::integral_constant<I>...>;

    /// Use this type if an function should be created only if the
    ///	current swizzle is writeable.
    typedef typename std::conditional<IsWritable, SwizzleProxy2, struct OperationNotAvailable>::type WriteableThisType;
    using WriteableThisType = std::conditional<IsWritable, SwizzleProxy2, struct OperationNotAvailable>::type;

    // You could make indexing beyond the size of the underlying thing fail:
    constexpr auto N = sizeof...(I);
    bool ok = (I < N && ...)
  };
