/*
$HOME/bin/bin/g++ -std=gnu++2a -g -Wall -Wextra -Wno-psabi -I. -o test_span test_span.cpp
*/

#include <span>
#include <array>
#include <vector>

int
main()
{
  int C[66];
  std::span<int> sC(C);

  std::array<double, 10> gmunu;
  std::span<double, 10> smunu(gmunu);

  std::vector<int> v{1,2,3,4,5};
  std::span<int, 5> sv(v);
}
