#pragma once

#include <thread>

class JoinThread
{
 public:
  JoinThread() noexcept = default;

  explicit JoinThread(std::thread t) noexcept : th_(std::move(t)) {}

  template <typename Callable, typename... Args>
  JoinThread(Callable&& func, Args&&... args) noexcept
      : th_(std::forward<Callable>(func), std::forward<Args>(args)...)
  {
  }

  JoinThread& operator=(std::thread&& other) noexcept
  {
    if (joinable())
    {
      join();
    }

    th_ = std::move(other);
    return *this;
  }

  ~JoinThread()
  {
    if (joinable())
    {
      join();
    }
  }

  void swap(JoinThread& other) { th_.swap(other.th_); }

  bool joinable() const { return th_.joinable(); }

  void join() { th_.join(); }

  void detch() { th_.detach(); }

  std::thread::id getId() const noexcept { return th_.get_id(); }

  std::thread& data() noexcept { return th_; }

  const std::thread& data() const noexcept { return th_; }

 private:
  std::thread th_;
};