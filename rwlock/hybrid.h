#include <thread>
#include <atomic>
#include <shared_mutex>

class HybridLocker {
private:
  std::shared_mutex mutex_;
  std::atomic_int64_t version_;
  std::atomic_bool isExclusive_;

  template<typename Functor, class T>
  bool tryReadOptimistically(Functor readCallback, T &arg) {
    if (this->isExclusive_) return false;
    auto preVersion = this->version_.load();

    readCallback(arg);
    if (this->isExclusive_) return false;
    return preVersion == this->version_.load();
  }

public:
  void ReadLock() { this->mutex_.lock_shared(); }
  void ReadUnlock() { this->mutex_.unlock_shared(); }
  void WriteLock() {
    this->mutex_.lock();
    this->isExclusive_ = true;
  }
  void WriteUnlock() { ++this->version_; this->isExclusive_ = false; this->mutex_.unlock(); }

  template<typename Functor, class T>
  void OptimisticRead(Functor readCallback, T &arg) {
    if (!this->tryReadOptimistically(readCallback, arg)) {
      this->ReadLock();
      readCallback(arg);
      this->ReadUnlock();
    }
  }  
};
