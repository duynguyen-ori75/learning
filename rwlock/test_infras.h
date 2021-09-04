extern "C" {
#include "simple_rwlock.h"
#include "rwlock.h"
#include "pthread.h"
}

#include "hybrid.h"

#include <functional>
#include <memory>

#define NO_THREADS 1000

#define TEST_PACKAGE(locker_type, RLockFunc, RUnlockFunc, WLockFunc, WUnlockFunc) \
  class Counter_##locker_type { \
    public: \
    locker_type   *lk; \
    int           counter_; \
    Counter_##locker_type(int default_counter) : counter_(default_counter), lk(new locker_type()) {}\
    ~Counter_##locker_type() { delete lk; } \
    void RLock() { RLockFunc(lk); } \
    void RUnlock() { RUnlockFunc(lk); } \
    void WLock() { WLockFunc(lk); } \
    void WUnlock() { WUnlockFunc(lk); } \
  } \

TEST_PACKAGE(RWLocker, ReadLock, ReadUnlock, WriteLock, WriteUnlock);
TEST_PACKAGE(SimpleRWLocker, SimpleRLock, SimpleRUnlock, SimpleWLock, SimpleWUnlock);
TEST_PACKAGE(pthread_rwlock_t, pthread_rwlock_rdlock, pthread_rwlock_unlock, pthread_rwlock_wrlock, pthread_rwlock_unlock);

class Counter_HybridLocker { 
public: 
  HybridLocker   *lk; 
  int           counter_; 
  Counter_HybridLocker(int default_counter) : counter_(default_counter), lk(new HybridLocker()) {}
  ~Counter_HybridLocker() { delete lk; }
  void RLock() { lk->ReadLock(); }
  void RUnlock() { lk->ReadUnlock(); }
  void WLock() { lk->WriteLock(); }
  void WUnlock() { lk->WriteUnlock(); }
};

template <class locker_T>
void *atomicAdd(void *arg) {
  auto *data = (locker_T*) arg;
  data->WLock();
  data->counter_ ++;
  data->WUnlock();
  pthread_exit(NULL);
}

void ReadCb(Counter_HybridLocker &object) {
  auto _ = object.counter_;
}

template <class locker_T>
void *atomicRead(void *arg) {
  auto *data = (locker_T*) arg;
  if constexpr(std::is_same_v<locker_T, Counter_HybridLocker>) {
    data->lk->template OptimisticRead<std::function<void(Counter_HybridLocker&)>, Counter_HybridLocker>(ReadCb, *data);
  } else {
    data->RLock();
    auto _ = data->counter_;
    data->RUnlock();
  }
  pthread_exit(NULL);
}