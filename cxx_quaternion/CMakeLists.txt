cmake_minimum_required (VERSION 3.2)

project(test_quaternion LANGUAGES CXX)

add_custom_target(make_cxx_quaternion_output_dir ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory output)

add_library(cxx_quaternion INTERFACE)
target_include_directories(cxx_quaternion INTERFACE include)

add_executable(test_quaternion test_quaternion.cpp)
target_link_libraries(test_quaternion cxx_quaternion)
add_test(NAME run_test_quaternion COMMAND bash -c "${CMAKE_BINARY_DIR}/bin/cxx_quaternion > output/cxx_quaternion.txt")
