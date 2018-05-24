#pragma once
#include <sys/time.h>
#include <chrono>
auto  get_time() {
    // struct timeval tv;
    // gettimeofday(&tv,0);
    // return (double) tv.tv_sec+tv.tv_usec*1e-6;
  return std::chrono::high_resolution_clock::now();
}
