# Simple RWLock implementation in C

Inspired by this excellent blog: https://eli.thegreenplace.net/2019/implementing-reader-writer-locks/

## Implementation details

- Three versions: `SimpleRWLocker`, `RWLocker`, and `HybridLocker`
- `SimpleRWLocker` is implemented using a simple `pthread_mutex_t`
- `RWLocker` use several techniques
  - two conditional variables - `reader_` & `writer_` - to notify the threads to start the execution
  - a bool `writer_entered_` to notify all threads that a write thread is ready to run
    - no read thread can execute after the toggle
- `HybridLocker` is from this paper: [Scalable and Robust Latches for Database Systems](https://dl.acm.org/doi/abs/10.1145/3399666.3399908)
- Compare these three with built-in read-write lock from pthread
- Test and benchmark are done in C++

## Compile command

```shell
make
```

## Requirements

- [googletest](https://github.com/google/googletest)
- [googlebenchmark](https://github.com/google/benchmark)

## Benchmark

### 4 threads

```shell
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
BM_RWLocker_ReadIntensive             34020 ns        19550 ns        40866
BM_HybridLocker_ReadIntensive         35205 ns        20217 ns        35681
BM_SimpleRWLocker_ReadIntensive       35684 ns        21234 ns        32973
BM_PthreadRWLock_ReadIntensive        35114 ns        21007 ns        32904
BM_RWLocker_WriteIntensive            35056 ns        21246 ns        34036
BM_HybridLocker_WriteIntensive        33186 ns        19392 ns        34427
BM_SimpleRWLocker_WriteIntensive      33026 ns        19875 ns        33351
BM_PthreadRWLock_WriteIntensive       33304 ns        20293 ns        46964
```

### 10 threads

```shell
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
BM_RWLocker_ReadIntensive            236722 ns       165869 ns         4378
BM_HybridLocker_ReadIntensive        228730 ns       164839 ns         4213
BM_SimpleRWLocker_ReadIntensive      250744 ns       159181 ns         4251
BM_PthreadRWLock_ReadIntensive       239502 ns       171455 ns         4078
BM_RWLocker_WriteIntensive           232761 ns       167941 ns         4195
BM_HybridLocker_WriteIntensive       236332 ns       167513 ns         4183
BM_SimpleRWLocker_WriteIntensive     235075 ns       168567 ns         4101
BM_PthreadRWLock_WriteIntensive      231884 ns       166195 ns         4118
```

### 100 threads

```shell
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
BM_RWLocker_ReadIntensive           1238986 ns      1077436 ns          750
BM_HybridLocker_ReadIntensive       1281932 ns      1086153 ns          646
BM_SimpleRWLocker_ReadIntensive     1250826 ns      1088703 ns          629
BM_PthreadRWLock_ReadIntensive      1251242 ns      1069169 ns          626
BM_RWLocker_WriteIntensive          1258623 ns      1099796 ns          625
BM_HybridLocker_WriteIntensive      1305213 ns      1107001 ns          628
BM_SimpleRWLocker_WriteIntensive    1262476 ns      1100092 ns          642
BM_PthreadRWLock_WriteIntensive     1275952 ns      1083896 ns          624
```

### 1000 threads

```shell
---------------------------------------------------------------------------
Benchmark                                 Time             CPU   Iterations
---------------------------------------------------------------------------
BM_RWLocker_ReadIntensive          10818504 ns     10279465 ns           75
BM_HybridLocker_ReadIntensive      11361333 ns     10438716 ns           68
BM_SimpleRWLocker_ReadIntensive    10610572 ns      9956991 ns           69
BM_PthreadRWLock_ReadIntensive     10585662 ns     10072601 ns           71
BM_RWLocker_WriteIntensive         10808241 ns     10205705 ns           65
BM_HybridLocker_WriteIntensive     10663432 ns     10138078 ns           66
BM_SimpleRWLocker_WriteIntensive   11185471 ns     10467922 ns           68
BM_PthreadRWLock_WriteIntensive    10810974 ns     10215760 ns           65
```