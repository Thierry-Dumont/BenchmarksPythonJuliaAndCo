cmake_minimum_required(VERSION 2.6)
project(BenchmarksPythonJuliaAndCo)
enable_language(CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -DXTENSOR_USE_XSIMD=1  -g -std=c++14 -Wall -march=native")

include_directories(
$ENV{HOME}/anaconda3/include/
) 
add_executable(
  run
  ../main.cpp
  )




