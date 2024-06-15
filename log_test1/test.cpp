#include "log.h"
#include <iostream>
#include <chrono>

using namespace logger;
int main() {
  for (int i = 0; i < 1000; i++) {
    LOG(INFO) << 1 << 2.1f << -3 << -4;
  }
  {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; i++) {
      LOG(INFO) << 1 << 2.1f << -3 << -4;
    }
    auto end = std::chrono::steady_clock::now();
    double elapsed_secs = double((end - start).count()) / (CLOCKS_PER_SEC);
    std::cout << "Elapsed time: " << elapsed_secs << "s\n";
  }
}
