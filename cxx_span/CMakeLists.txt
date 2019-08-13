cmake_minimum_required (VERSION 3.2)

project(cxx_span LANGUAGES CXX)

add_library(cxx_span INTERFACE)
target_include_directories(cxx_span INTERFACE include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-psabi -Wno-deprecated-declarations -Wold-style-cast")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(test_span test_span.cpp)
target_link_libraries(test_span cxx_span)
add_test(NAME run_test_span COMMAND bash -c "${CMAKE_BINARY_DIR}/bin/test_span > output/test_span.txt")

add_executable(test_array_ref test_array_ref.cpp)
target_link_libraries(test_array_ref cxx_span)
add_test(NAME run_test_array_ref COMMAND bash -c "${CMAKE_BINARY_DIR}/bin/test_array_ref > output/test_array_ref.txt")
