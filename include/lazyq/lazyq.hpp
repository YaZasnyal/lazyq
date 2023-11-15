#pragma once

#include <atomic>
#include <mutex>
#include <queue>

namespace lazyq
{

template<class T, class Locker>
class queue_type
{
public:
  void push(T&& data)
  {
    std::unique_lock lock(mutex_);
    queue_.push(std::move(data));
  }

  T pop()
  {
    std::unique_lock lock(mutex_);
    T temp = std::move(queue_.front());
    queue_.pop();
    return temp;
  }

private:
  Locker mutex_;
  std::queue<T> queue_;
};

class null_mutex
{
public:
  void lock() noexcept {}
  void unlock() noexcept {}
  bool try_lock() noexcept { return true; }
};

template<class MessageType>
class lazyq_interface
{
public:
  lazyq_interface() = default;
  lazyq_interface(const lazyq_interface<MessageType>&) = default;
  lazyq_interface(lazyq_interface<MessageType>&&) = default;
  lazyq_interface& operator=(const lazyq_interface<MessageType>&) = default;
  lazyq_interface& operator=(lazyq_interface<MessageType>&&) = default;
  virtual ~lazyq_interface() noexcept = default;
  
  virtual void post_message(MessageType&& data) noexcept = 0;
  virtual void process_message(MessageType&& data) noexcept = 0;
};

template<class Processor,
         class MessageType,
         class Queue = lazyq::queue_type<MessageType, std::mutex>>
class lazyq : public lazyq_interface<MessageType>
{
public:
  void post_message(MessageType&& data) noexcept final
  {
    unsigned expected = 0;
    if (!inflight_.compare_exchange_strong(
            expected, 1, std::memory_order_acquire, std::memory_order_relaxed))
    {
      data.to_owned();

      queue_.push(std::move(data));
      auto inflight = inflight_.fetch_add(1, std::memory_order_acquire);
      if (inflight != 0) {
        // other thread is already working
        return;
      }
    } else {
      if (post_message_fast(std::move(data))) {
        return;
      }
    }

    while (true) {
      MessageType queued_data = queue_.pop();
      static_cast<Processor*>(this)->process_message(std::move(queued_data));
      if (inflight_.fetch_sub(1, std::memory_order_release) == 1) {
        break;
      }
    }
  }

private:
  // fast path without pushing to the queue
  // returns true if there is no work to be done
  inline bool post_message_fast(MessageType&& data) noexcept
  {
    static_cast<Processor*>(this)->process_message(std::move(data));
    return inflight_.fetch_sub(1, std::memory_order_release) == 1;
  }

  Queue queue_;
  std::atomic_uint inflight_ = ATOMIC_VAR_INIT(0);
};

}  // namespace lazyq