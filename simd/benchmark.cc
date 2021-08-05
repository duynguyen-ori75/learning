#include <vector>
#include <iostream>
#include <assert.h>

#include <benchmark/benchmark.h>

#include "xsimd/xsimd.hpp"

#define NO_ELEMENTS   1000000

using vector_simd = std::vector<int, xsimd::aligned_allocator<int, XSIMD_DEFAULT_ALIGNMENT>>;
constexpr std::size_t simd_size = xsimd::simd_type<int>::size;
auto data = vector_simd(NO_ELEMENTS, 1);

static void BM_TraditionalLoop(benchmark::State& state) {
  for (auto _ : state) {
    int sum = 0;
    for(int idx = 0; idx < NO_ELEMENTS; idx ++) {
      sum += data[idx];
    }
    assert(sum == NO_ELEMENTS);
  }
}

static void BM_SIMDLoop(benchmark::State& state) {
  xsimd::batch<int, simd_size> sum;
  for (auto _ : state) {
    sum = xsimd::batch<int, simd_size>(0);
    for(auto idx = 0; idx < NO_ELEMENTS; idx += simd_size) {
      auto elem = xsimd::load_aligned(&data[idx]);
      sum += elem;
    }
    for(auto idx = 1; idx < simd_size; idx++) {
      sum[0] += sum[idx];
    }
    assert(sum[0] == NO_ELEMENTS);
  }
}

static void BM_SIMDReduce(benchmark::State& state) {
  for (auto _ : state) {
    int sum = xsimd::reduce(data.begin(), data.end(), 0);
    assert(sum == NO_ELEMENTS);
  }
}

BENCHMARK(BM_TraditionalLoop);
BENCHMARK(BM_SIMDLoop);
BENCHMARK(BM_SIMDReduce);

BENCHMARK_MAIN();