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
find_package(xsimd) 
if (${CMAKE_CXX_COMPILER} MATCHES "icpc.*$")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -DICC -DALIGN_64 -restrict -O3  -g -xavx -ipo -fargument-noalias  -ansi-alias -Wall -vec-report3 -std=c++0x")


else ()
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall  -DXTENSOR_USE_XSIMD   -std=c++17 -march=native -mtune=native")

endif ()
include_directories(
  $ENV{HOME}/anaconda3/include/
  ${xsimd_INCLUDE_DIR}
   )

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


