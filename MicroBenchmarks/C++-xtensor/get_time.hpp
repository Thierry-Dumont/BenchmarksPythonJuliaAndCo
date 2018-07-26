#pragma once
#include <sys/time.h>
#include <chrono>
auto  get_time() {
  return std::chrono::high_resolution_clock::now();
}
