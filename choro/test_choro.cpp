/*
$HOME/bin/bin/g++ -std=c++17 -Wall -Wextra -o test_choro test_choro.cpp
./test_choro
*/

#include <iostream>

#include "choro"

int
main()
{
  pt2f a{1.0, 2.0}, b{2.0, -1.0};
  auto vab = b - a;
  std::cout << a << '\n';
  std::cout << b << '\n';
  std::cout << vab << '\n';
}
