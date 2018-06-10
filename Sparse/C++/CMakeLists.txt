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

if (${CMAKE_CXX_COMPILER} MATCHES "icpc.*$")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -DICC -DALIGN_64 -restrict -O3  -g -xavx -ipo -fargument-noalias  -ansi-alias -Wall -vec-report3 -std=c++0x")

elseif(${CMAKE_CXX_COMPILER} MATCHES "clang.*$")
  set(CMAKE_CXX_COMPILER "clang++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3  -Wall -DCLANG  -Wall -std=c++14 -march=native")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall  -std=c++14 -Wall -march=native")

endif ()

add_executable(
  run
  ../main.cpp
  )




