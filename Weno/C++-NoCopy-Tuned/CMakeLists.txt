cmake_minimum_required(VERSION 2.6)
project(BenchmarksPythonJuliaAndCo)
enable_language(CXX)
# Go to Build directory. Then:
#
#- To use intel compiler
# CXX=icpc cmake ..
# - for clang++:
# CXX=clang++ cmake ..
# - otherwise, to use g++:
#  cmake ..
#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -Wall -std=c++14 -march=native -mtune=native")


add_executable(
  run
  ../main.cpp
)




