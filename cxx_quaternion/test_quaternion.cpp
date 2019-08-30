
#include <iostream>

#include <ext/quaternion.h>

int
main()
{
  __gnu_cxx::quaternion<double> q0;
  std::cout << "q0 = " << q0 << '\n';

  __gnu_cxx::quaternion<double> q1(1.0, 0.707, 0.0, 0.707);
  std::cout << "q1 = " << q1 << '\n';
}
