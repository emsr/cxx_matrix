/*
$HOME/bin/bin/g++ -std=gnu++2a -g -Wall -Wextra -Wno-psabi -I. -o test_span test_span.cpp
*/

#include <span>
#include <array>
#include <vector>
#include <cassert>
/*
constexpr bool
test_span()
{
  int C[66]{};
  std::span<int> sC(C);
  sC.size() == 66;
  sC.size_bytes() == 66 * sizeof(int);
  int sum = 0;
  for (const auto& c : sC)
    sum += c;
  int k = 0;
  for (auto& c : sC)
    c = ++k;
  //auto bC = std::as_bytes(sC);
  //auto wbC = std::as_writeable_bytes(sC);

  constexpr std::array<double, 16>
  gmunu{-1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1};
  constexpr std::span<double, 10> smunu(gmunu);
  static_assert(!smunu.empty());
  static_assert(smunu.size() == 16);
  static_assert(smunu.size_bytes() == 16 * sizeof(double));

  constexpr std::array<int,5> a{1,2,3,4,5};
  constexpr std::span<int, 5> sa(a);
  static_assert(sa.size() == 5);
  static_assert(sa.size_bytes() == 5 * sizeof(int));

  return true;
}
*/
int
main()
{
  int C[66]{};
  std::span<int,66> sC(C);
  assert(sC.size() == 66);
  assert(sC.size_bytes() == 66 * sizeof(int));
  int k = 0;
  for (auto& c : sC)
    c = ++k;
  int sum = 0;
  for (const auto& c : sC)
    sum += c;
  auto bC = std::as_bytes(sC);
  auto wbC = std::as_writeable_bytes(sC);

  std::array<double, 10> gmunu;
  std::span<double, 10> smunu(gmunu);

  std::vector<int> v{1,2,3,4,5};
  std::span<int, 5> sv(v);
  assert(sv.size() == 5);
  assert(sv.size_bytes() == 5 * sizeof(int));
  assert(sv.front() == 1);
  assert(sv.back() == 5);
}
