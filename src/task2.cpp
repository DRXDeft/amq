#include "BooPHF.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;
typedef boomphf::SingleHashFunctor<uint64_t> hasher_t;
typedef boomphf::mphf<uint64_t, hasher_t> boophf_t;
inline uint32_t hash32(uint32_t a) {
  a = (a + 0x7ed55d16) + (a << 12);
  a = (a ^ 0xc761c23c) ^ (a >> 19);
  a = (a + 0x165667b1) + (a << 5);
  a = (a + 0xd3a2646c) ^ (a << 9);
  a = (a + 0xfd7046c5) + (a << 3);
  a = (a ^ 0xb55a4f09) ^ (a >> 16);
  if (a < 0)
    a = -a;
  return a;
}
double k_value[] = {1000, 10000, 100000, 1000000};
int main() {
  for (int i = 0; i < 4; i++) {
    std::vector<uint32_t> input_keys;
    for (int j = 0; j < k_value[i]; j++)
      input_keys.push_back(hash32(j));
    boophf_t *bphf =
        new boomphf::mphf<uint64_t, hasher_t>(input_keys.size(), input_keys, 8);
    uint32_t counter = 0;
    cout << "runing test for " << k_value[i] << endl;
    auto timerstart = chrono::high_resolution_clock::now();
    for (int j = 0; j < k_value[i]; j++) {
      uint32_t idx;
      if (j % 2) {
        uint32_t idx = bphf->lookup(hash32(input_keys[j]));
        if (idx < input_keys.size())
          counter++;
      } else
        uint32_t idx = bphf->lookup(input_keys[j]);
    }
    auto timerstop = chrono::high_resolution_clock::now();
    auto running_time =
        chrono::duration_cast<chrono::microseconds>(timerstop - timerstart);
    printf("False_Positive Rate is %.8f\n", counter * 1.0 / k_value[i]);
    cout << "running time is " << running_time.count() << "microseconds"
         << endl;
  }
}