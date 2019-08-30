
#include <span>
#include <array>
#include <algorithm>

void
test_incompatible_type()
{
  std::array<int, 4> src{1, 2, 3, 4};
  std::array<int*, 12> dst{};

  std::span<int> src_span_dyn(src);
  std::span<int, 4> src_span_static(src);
  std::span<int*> dst_span_dyn(dst);
  std::span<int*, 4> dst_span_static(dst);

  // Every line should produce a compilation error
  std::copy(src_span_dyn.begin(), src_span_dyn.end(), dst_span_dyn.begin());
  std::copy(src_span_dyn.begin(), src_span_dyn.end(), dst_span_static.begin());
  std::copy(src_span_static.begin(), src_span_static.end(), dst_span_dyn.begin());
  std::copy(src_span_static.begin(), src_span_static.end(), dst_span_static.begin());
}
