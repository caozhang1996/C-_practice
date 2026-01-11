#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <vector>

// 线程池配置结构体
struct ThreadPoolConfig
{
  size_t min_threads;                    // 最小线程数
  size_t max_threads;                    // 最大线程数
  size_t queue_size_limit;               // 任务队列大小限制
  bool enable_affinity;                  // 是否启用线程亲和性
  std::chrono::seconds keep_alive_time;  // 空闲线程存活时间

  // 构造函数，提供合理默认值
  ThreadPoolConfig();
};

class ThreadPool
{
 public:
  // 构造函数
  explicit ThreadPool(const ThreadPoolConfig& config);
  // 默认构造函数，使用默认配置
  ThreadPool();

  // 禁止拷贝和移动
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  // 析构函数
  ~ThreadPool();

  // 提交任务并返回future
  template <class F, class... Args>
  auto submit(F&& f, Args&&... args)
      -> std::future<std::result_of_t<F(Args...)>>;

  // 获取当前活跃线程数
  size_t active_threads() const;

  // 获取当前任务队列大小
  size_t queue_size() const;

  // 停止线程池
  void stop();

  // 检查线程池是否正在运行
  bool is_running() const;

 private:
  // 任务接口
  struct TaskBase
  {
    virtual ~TaskBase() = default;
    virtual void execute() = 0;
  };

  // 具体任务类型
  template <class F>
  struct Task : TaskBase
  {
    F func;
    explicit Task(F&& f) : func(std::forward<F>(f)) {}
    void execute() override { func(); }
  };

  // 工作线程函数
  void worker_thread(size_t thread_id);

  // 设置线程亲和性
  void set_thread_affinity(size_t thread_id);

  // 设置线程名称
  void set_thread_name(const std::string& name);

  // 从队列获取任务（包括任务窃取）
  std::unique_ptr<TaskBase> get_task(size_t thread_id);

  // 检查是否有可窃取的任务
  bool has_stealable_tasks() const;

  // 调整线程数量（根据任务负载）
  void adjust_thread_count();

  ThreadPoolConfig config_;           // 配置
  std::vector<std::thread> threads_;  // 工作线程
  std::vector<std::deque<std::unique_ptr<TaskBase>>>
      local_queues_;                       // 本地任务队列
  mutable std::mutex global_mutex_;        // 全局互斥锁
  std::condition_variable global_cv_;      // 全局条件变量
  std::atomic<bool> is_running_{false};    // 线程池是否运行
  std::atomic<size_t> active_threads_{0};  // 活跃线程数
  std::atomic<size_t> total_tasks_{0};     // 总任务数
};

// 模板方法实现
template <class F, class... Args>
auto ThreadPool::submit(F&& f, Args&&... args)
    -> std::future<std::result_of_t<F(Args...)>>
{
  using return_type = std::result_of_t<F(Args...)>;

  // 包装任务为packaged_task
  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  // 获取future
  std::future<return_type> res = task->get_future();

  // 检查线程池状态
  if (!is_running_)
  {
    throw std::runtime_error("线程池已停止，无法提交任务");
  }

  // 检查队列大小限制
  if (config_.queue_size_limit > 0 && total_tasks_ >= config_.queue_size_limit)
  {
    throw std::runtime_error("任务队列已满，无法提交新任务");
  }

  // 选择一个本地队列（简单的轮询策略）
  static std::atomic<size_t> next_queue_{0};
  size_t queue_id = next_queue_++ % local_queues_.size();

  // 将任务加入队列
  {
    std::lock_guard<std::mutex> lock(global_mutex_);
    local_queues_[queue_id].push_back(
        std::make_unique<Task<std::function<void()>>>([task]() { (*task)(); }));
    total_tasks_++;
  }

  // 唤醒一个工作线程
  global_cv_.notify_one();

  // 可能需要调整线程数量
  adjust_thread_count();

  return res;
}
