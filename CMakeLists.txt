cmake_minimum_required (VERSION 3.10)

project(
  cxx_matrix
  VERSION 0.5
  LANGUAGES CXX
)

enable_testing()

add_subdirectory(cxx_span)
add_subdirectory(cxx_matrix_math)
add_subdirectory(cxx_quaternion)

