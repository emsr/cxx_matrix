/*
$HOME/bin/bin/g++ -std=gnu++2a -g -Wall -Wextra -Wno-psabi -I. -o test_span test_span.cpp
*/

#include <span>
#include <array>
#include <vector>
#include <cassert>
#include <algorithm>

void
test_same_type()
{
  // dynamic source and destination span
  {
    std::array<int, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<int> src_span(src);
    const std::span<int> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }

  // static source and dynamic destination span
  {
    std::array<int, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<int, 5> src_span(src);
    const std::span<int> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }

  // dynamic source and static destination span
  {
    std::array<int, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<int> src_span(src);
    const std::span<int, 10> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }

  // static source and destination span
  {
    std::array<int, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<int, 5> src_span(src);
    const std::span<int, 10> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }
}


void
test_compat_type()
{
  // dynamic source and destination span
  {
    std::array<short, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<short> src_span(src);
    const std::span<int> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }

  // static source and dynamic destination span
  {
    std::array<short, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<short, 5> src_span(src);
    const std::span<int> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }

  // dynamic source and static destination span
  {
    std::array<short, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<short> src_span(src);
    const std::span<int, 10> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }

  // static source and destination span
  {
    std::array<short, 5> src{1, 2, 3, 4, 5};
    std::array<int, 10> dst{};

    const std::span<short, 5> src_span(src);
    const std::span<int, 10> dst_span(dst);

    std::copy(src_span.begin(), src_span.end(), dst_span.begin());
    std::copy(src_span.begin(), src_span.end(), dst_span.subspan(src_span.size()).begin());

    for (std::size_t i = 0; i < src.size(); ++i)
      {
	assert(dst[i] == src[i]);
	assert(dst[i + src.size()] == src[i]);
      }
  }
}

void
test_small_destination_span()
{
  std::array<int, 12> src{1, 2, 3, 4};
  std::array<int, 4> dst{};

  const std::span<int> src_span_dyn(src);
  const std::span<int, 12> src_span_static(src);
  const std::span<int> dst_span_dyn(dst);
  const std::span<int, 4> dst_span_static(dst);

  //CHECK_THROWS_AS(std::copy(src_span_dyn.begin(), src_span_dyn.end(), dst_span_dyn.begin()), fail_fast);
  //CHECK_THROWS_AS(std::copy(src_span_dyn.begin(), src_span_dyn.end(), dst_span_static.begin()), fail_fast);
  //CHECK_THROWS_AS(std::copy(src_span_static.begin(), src_span_static.end(), dst_span_dyn), fail_fast);

  // This should bomb.
  std::copy(src_span_static.begin(), src_span_static.end(), dst_span_static.begin());
}
int
main()
{
  int C[66];
  std::span<int> sC(C);

  std::array<double, 10> gmunu;
  std::span<double, 10> smunu(gmunu);

  std::vector<int> v{1,2,3,4,5};
  std::span<int, 5> sv(v);

  test_same_type();
  test_compat_type();
}
