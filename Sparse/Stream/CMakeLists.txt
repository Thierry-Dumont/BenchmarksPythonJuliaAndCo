cmake_minimum_required(VERSION 2.6)
project(bla)
#
# To use intel compiler:
# CXX=icpc  cmake <path to this file>
# otherwise, to use g++:
#  cmake <path to this file>
#

if (${CMAKE_CXX_COMPILER} MATCHES "icpc.*$")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3  -gcc-name=gcc-4.7  -mcmodel=large -DICC -DALIGN_BLOCKED -DALIGN_64 -openmp -restrict  -std=c++11  -vec-report0 -g -xavx -fargument-noalias   -ansi-alias -mcmodel=large -opt-subscript-in-range")


 include_directories(
.
/opt/intel/vtune_amplifier_xe/include/
/usr/local/intel/vtune_amplifier_xe_2016.1.0.424694/include/
)
link_directories(
/opt/intel/vtune_amplifier_xe/lib64/
/usr/local/intel/vtune_amplifier_xe_2016.1.0.424694/lib64/
)

add_executable(
	stream
	./stream.c
)

target_link_libraries(
	stream
	mkl_intel_lp64
	mkl_sequential
	mkl_core
	mkl_lapack95_lp64
	mkl_blas95_lp64
	#ittnotify
)


else() 
#  link_directories(${CMAKE_SOURCE_DIR}/tbb/lib/)
  #set(CMAKE_CXX_COMPILER "g++-5")
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3 -fopenmp  -m arch=native")
  

  include_directories(
  .

)
link_directories()
add_executable(
	stream
	./stream.c
)

target_link_libraries(
  stream

	)
    
file(GLOB_RECURSE to_remove "*.to")
set_directory_properties(PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES "${to_remove}" ) 

endif()  
  



