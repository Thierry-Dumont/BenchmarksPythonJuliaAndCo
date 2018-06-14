cmake_minimum_required(VERSION 2.6)
project(BenchmarksPythonJuliaAndCo)
enable_language(Fortran)
# 
set(CMAKE_fortran_COMPILER "gfortran")

  set(CMAKE_fortran_FLAGS "${CMAKE_Fortran_FLAGS} -Wall   -O3 -Wall -march=native")


add_executable(
  run
  ../main.F90
  ../m_burghers.F90  ../m_godunov.F90  ../m_RK3TVDData.F90  ../m_weno.F90
  )





