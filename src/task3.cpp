#include "BooPHF.h"
#include "sdsl/int_vector.hpp"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
typedef boomphf::SingleHashFunctor<uint32_t> hasher_t;
typedef boomphf::mphf<uint32_t, hasher_t> boophf_t;
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
void transform(char *array, uint32_t key) {
  for (int i = 0; i < 32; i++)
    array[i] = (key % (i + 1)) % 26 + 'a';
}
double k_value[] = {1000, 10000, 100000, 1000000};
int B[] = {7, 8, 10};
int main() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      sdsl::int_vector<> aug_filter(k_value[i], 0, B[j]);
      std::vector<uint32_t> input_keys;

      for (int k = 0; k < k_value[i]; k++) {

        // transform(array, hash32(k));
        input_keys.push_back(hash32(k));
      }

      boophf_t *bphf = new boomphf::mphf<uint32_t, hasher_t>(input_keys.size(),
                                                             input_keys, 8);
      uint32_t Max = 0;
      for (int k = 0; k < k_value[i]; k++)
        Max = Max > bphf->lookup(input_keys[k]) ? Max
                                                : bphf->lookup(input_keys[k]);

      for (int k = 0; k < k_value[i]; k++) {
        auto idx = bphf->lookup(input_keys[k]);
        // cout << idx << " " << aug_filter.size() << endl;
        if (idx >= aug_filter.size())
          continue;
        aug_filter[bphf->lookup(input_keys[k])] =
            std::hash<uint32_t>{}(input_keys[k] & ((2 << B[j]) - 1));
      }
      // cout << Max << endl;
      cout << "runing test for " << k_value[i] << " bit " << B[j] << endl;
      auto timerstart = chrono::high_resolution_clock::now();

      size_t counter = 0;
      for (int k = 0; k < k_value[i]; k++) {
        if (k % 2) {
          uint32_t idx = bphf->lookup(hash32(input_keys[k]));
          if (idx >= aug_filter.size())
            continue;
          if (aug_filter[idx] ==
              std::hash<uint32_t>{}(input_keys[k] & ((2 << B[j]) - 1)))
            counter++;
        }
      }
      auto timerstop = chrono::high_resolution_clock::now();
      auto running_time =
          chrono::duration_cast<chrono::microseconds>(timerstop - timerstart);
      printf("False_Positive Rate is %.8f\n", counter * 1.0 / k_value[i]);
      cout << "running time is " << running_time.count() << "microseconds"
           << endl;
    }
  }
}
