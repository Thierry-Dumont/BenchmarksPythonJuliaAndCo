cmake_minimum_required(VERSION 2.6)
project(NewRadau5)
enable_language(CXX Fortran)
# Go to Build directory. Then:
# To use intel compiler
# CXX=icpc cmake ..
# for clang++:
# CXX=clang++ cmake ..
# otherwise, to use g++:
#  cmake ..
#

if (${CMAKE_CXX_COMPILER} MATCHES "icpc.*$")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -DICC -DALIGN_64 -restrict -O3  -g -xavx -ipo -fargument-noalias  -ansi-alias -Wall -vec-report3 -std=c++0x")


else ()
  set (USING_GNU TRUE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall  -DGCC -DALIGN_64  -std=c++14 -march=native")

endif ()

add_executable(
  run_cl
  ../main_cl.cpp
  )
add_executable(
  run_lapl_1
  ../main_lapl_1.cpp
  )
add_executable(
  run_lapl_2
  ../main_lapl_2.cpp
  )


