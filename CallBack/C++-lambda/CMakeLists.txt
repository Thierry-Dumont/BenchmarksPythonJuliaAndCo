cmake_minimum_required(VERSION 2.6)
project(Bench)
enable_language(CXX)
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall  -DGCC  -std=c++14 -march=native")

endif ()
include_directories(
${CMAKE_SOURCE_DIR}/ARRAY/include
) 
add_executable(
  run
  ../main.cpp
  )




