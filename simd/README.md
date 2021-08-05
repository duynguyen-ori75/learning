# SIMD playground

Entering the SIMD world....

Quick benchmark between SIMD vs simple loop

## Requirements

- [googletest](https://github.com/google/googletest)
- [googlebenchmark](https://github.com/google/benchmark)
- [xsimd](https://github.com/xtensor-stack/xsimd)

## Benchmarks

Surprisingly, the verdicts are divergent for different `-O` compile flag values

Perhaps this is a perfect time to learn assembly ??

```shell
g++ -O0 ...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_TraditionalLoop    2028300 ns      2028268 ns          345
BM_SIMDLoop           8767717 ns      8767344 ns           79
BM_SIMDReduce         6069143 ns      6068764 ns          111
```

```shell
g++ -O1 ...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_TraditionalLoop     503914 ns       503906 ns         1083
BM_SIMDLoop             64712 ns        64711 ns         9825
BM_SIMDReduce           66265 ns        66263 ns         9539
```

```shell
g++ -O2 ...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_TraditionalLoop     336621 ns       336614 ns         2116
BM_SIMDLoop             62358 ns        62357 ns        10384
BM_SIMDReduce           63308 ns        63308 ns         9818
```

```shell
g++ -O3 ...
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_TraditionalLoop      66531 ns        66531 ns        10186
BM_SIMDLoop             66413 ns        66412 ns         9420
BM_SIMDReduce           66217 ns        66211 ns         9519
```