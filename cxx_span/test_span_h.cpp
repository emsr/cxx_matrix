/*
$HOME/bin/bin/g++ -std=c++2a -o test_span_x test_span_x.cpp
*/

#include "span.h"

int
main()
{
  std::array<double, 10> gmunu;
  int C[66];

  std::span<double, std::ptrdiff_t = 10> smunu(gmunu);
}
