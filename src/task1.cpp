// #include "../include/libbloom/bloom.h"
#include "bloom.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
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

double false_positive_rate[] = {0.0078125, 0.00390625, 0.000976562};
double k_value[] = {1000, 10000, 100000, 1000000};
void transform(char *array, uint32_t key) {
  for (int i = 0; i < 32; i++)
    array[i] = (key % (i + 1)) % 26 + 'a';
}
int main() {

  for (int i = 0; i < 3; i++) {
    for (int k = 0; k < 4; k++) {
      cout << "False Positive Rate: " << false_positive_rate[i]
           << " k:" << k_value[k] << endl;
      struct bloom bloom;
      bloom_init2(&bloom, k_value[k], false_positive_rate[i]);
      std::vector<uint32_t> Keys;
      for (int j = 0; j < k_value[k]; j++) {
        Keys.push_back(hash32(j));
        char *array = new char[32];
        transform(array, Keys[j]);
        bloom_add(&bloom, array, 32);
      }
      uint32_t counter = 0;
      auto timerstart = chrono::high_resolution_clock::now();
      for (int j = 0; j < k_value[k]; j++) {
        char *array = new char[32];
        if (j % 2)
          transform(array, Keys[j]);
        else {

          transform(array, hash32(Keys[j]));
          if (bloom_check(&bloom, array, 32))
            counter++;
        }
      }
      auto timerstop = chrono::high_resolution_clock::now();
      auto running_time =
          chrono::duration_cast<chrono::microseconds>(timerstop - timerstart);
      printf("False_Positive Rate is %.8f\n", counter * 1.0 / k_value[k]);
      cout << "running time is " << running_time.count() << "microseconds"
           << endl;
    }
  }

  // if (bloom_check(&bloom, "buffer", 10)) {
  //   printf("It may be there!\n");
  // }

  return 0;
}
