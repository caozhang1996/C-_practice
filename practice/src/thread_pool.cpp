#include "practice/thread_pool.h"

#if defined(__linux__) && defined(_GNU_SOURCE)
// #include <cpu_set.h>
#include <pthread.h>
#include <sys/prctl.h>
#elif defined(_WIN32)
#include <processthreadsapi.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

// 配置结构体构造函数
ThreadPoolConfig::ThreadPoolConfig()
{
  // 获取硬件并发数作为基础
  size_t hardware_threads = std::thread::hardware_concurrency();
  if (hardware_threads == 0)
  {
    hardware_threads = 4;  // 无法获取时默认4线程
  }

  min_threads = hardware_threads;
  max_threads = hardware_threads * 2;
  queue_size_limit = 10000;
  enable_affinity = false;
  keep_alive_time = std::chrono::seconds(30);
}

// 线程池构造函数
ThreadPool::ThreadPool(const ThreadPoolConfig& config) : config_(config)
{
  try
  {
    // 验证配置有效性
    if (config_.min_threads == 0)
    {
      config_.min_threads = 1;
    }
    if (config_.max_threads < config_.min_threads)
    {
      config_.max_threads = config_.min_threads;
    }
    if (config_.keep_alive_time.count() <= 0)
    {
      config_.keep_alive_time = std::chrono::seconds(30);
    }

    // 初始化本地队列
    local_queues_.resize(config_.min_threads);

    // 标记线程池为运行状态
    is_running_ = true;

    // 创建初始工作线程
    for (size_t i = 0; i < config_.min_threads; ++i)
    {
      try
      {
        threads_.emplace_back(&ThreadPool::worker_thread, this, i);
      }
      catch (const std::exception& e)
      {
        std::cerr << "创建线程 " << i << " 失败: " << e.what() << std::endl;
        // 清理已创建的线程
        stop();
        throw;
      }
    }

    std::cout << "线程池初始化完成，初始线程数: " << threads_.size()
              << std::endl;
  }
  catch (...)
  {
    is_running_ = false;
    throw;
  }
}

// 默认构造函数
ThreadPool::ThreadPool() : ThreadPool(ThreadPoolConfig())
{
}

// 析构函数
ThreadPool::~ThreadPool()
{
  stop();
}

// 停止线程池
void ThreadPool::stop()
{
  if (!is_running_)
  {
    return;
  }

  // 标记为停止状态
  is_running_ = false;

  // 唤醒所有工作线程
  global_cv_.notify_all();

  // 等待所有线程结束
  for (auto& thread : threads_)
  {
    if (thread.joinable())
    {
      try
      {
        thread.join();
      }
      catch (const std::exception& e)
      {
        std::cerr << "等待线程结束失败: " << e.what() << std::endl;
      }
    }
  }

  threads_.clear();
  std::cout << "线程池已停止" << std::endl;
}

// 获取当前活跃线程数
size_t ThreadPool::active_threads() const
{
  return active_threads_;
}

// 获取当前任务队列大小
size_t ThreadPool::queue_size() const
{
  return total_tasks_;
}

// 检查线程池是否正在运行
bool ThreadPool::is_running() const
{
  return is_running_;
}

// 工作线程函数
void ThreadPool::worker_thread(size_t thread_id)
{
  try
  {
    // 设置线程名称和亲和性
    std::stringstream ss;
    ss << "worker-" << thread_id;
    set_thread_name(ss.str());

    if (config_.enable_affinity)
    {
      try
      {
        set_thread_affinity(thread_id);
      }
      catch (const std::exception& e)
      {
        std::cerr << "线程 " << thread_id << " 设置亲和性失败: " << e.what()
                  << std::endl;
        // 亲和性设置失败不影响线程运行，继续执行
      }
    }

    auto last_active_time = std::chrono::steady_clock::now();

    while (is_running_)
    {
      // 尝试获取任务
      std::unique_ptr<TaskBase> task = get_task(thread_id);

      if (task)
      {
        // 执行任务
        last_active_time = std::chrono::steady_clock::now();
        active_threads_++;

        try
        {
          task->execute();
        }
        catch (const std::exception& e)
        {
          std::cerr << "线程 " << thread_id << " 执行任务失败: " << e.what()
                    << std::endl;
        }
        catch (...)
        {
          std::cerr << "线程 " << thread_id << " 执行任务失败: 未知异常"
                    << std::endl;
        }

        active_threads_--;
        total_tasks_--;
      }
      else
      {
        // 没有任务，等待通知
        std::unique_lock<std::mutex> lock(global_mutex_);

        // 检查是否需要销毁这个线程（超过最大线程数且空闲时间过长）
        if (threads_.size() > config_.min_threads)
        {
          auto now = std::chrono::steady_clock::now();
          auto idle_time = std::chrono::duration_cast<std::chrono::seconds>(
              now - last_active_time);

          if (idle_time >= config_.keep_alive_time)
          {
            // 从线程列表中移除当前线程
            // 注意：这里需要小心处理，我们不能直接修改threads_容器
            // 所以我们只需要退出线程函数即可
            std::cout << "线程 " << thread_id << " 因长时间空闲被销毁"
                      << std::endl;
            return;
          }
        }

        // 等待新任务或超时
        global_cv_.wait_for(lock, std::chrono::seconds(1), [this, thread_id]() {
          return !is_running_ || !local_queues_[thread_id].empty() ||
                 has_stealable_tasks();
        });
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "线程 " << thread_id << " 异常退出: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "线程 " << thread_id << " 异常退出: 未知异常" << std::endl;
  }
}

// 设置线程亲和性
void ThreadPool::set_thread_affinity(size_t thread_id)
{
#if defined(__linux__) && defined(_GNU_SOURCE)
  // 获取系统CPU核心数
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);

  int num_cores = static_cast<int>(std::thread::hardware_concurrency());
  if (num_cores <= 0)
  {
    num_cores = 4;  // 无法获取时默认4核心
  }

  // 分配CPU核心（循环分配）
  int core_id = static_cast<int>(thread_id % num_cores);
  CPU_SET(core_id, &cpuset);

  // 设置线程亲和性
  pthread_t pthread = pthread_self();
  int rc = pthread_setaffinity_np(pthread, sizeof(cpu_set_t), &cpuset);
  if (rc != 0)
  {
    throw std::system_error(rc, std::system_category(), "设置线程亲和性失败");
  }

  std::cout << "线程 " << thread_id << " 绑定到CPU核心 " << core_id
            << std::endl;
#elif defined(_WIN32)
  // Windows系统设置线程亲和性
  DWORD_PTR mask = 1ULL << (thread_id % std::thread::hardware_concurrency());
  if (!SetThreadAffinityMask(GetCurrentThread(), mask))
  {
    throw std::system_error(GetLastError(), std::system_category(),
                            "设置线程亲和性失败");
  }
#else
  // 不支持的平台
  throw std::runtime_error("当前平台不支持线程亲和性设置");
#endif
}

// 设置线程名称
void ThreadPool::set_thread_name(const std::string& name)
{
#if defined(__linux__) && defined(_GNU_SOURCE)
  // Linux系统设置线程名称
  prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
#elif defined(_WIN32)
  // Windows系统设置线程名称
  std::wstring wname(name.begin(), name.end());
  SetThreadDescription(GetCurrentThread(), wname.c_str());
#elif defined(__APPLE__)
  // macOS系统设置线程名称
  pthread_setname_np(name.c_str());
#else
  // 不支持的平台忽略
#endif
}

// 从队列获取任务（包括任务窃取）
std::unique_ptr<ThreadPool::TaskBase> ThreadPool::get_task(size_t thread_id)
{
  std::lock_guard<std::mutex> lock(global_mutex_);

  // 先检查本地队列
  if (!local_queues_[thread_id].empty())
  {
    auto task = std::move(local_queues_[thread_id].front());
    local_queues_[thread_id].pop_back();
    return task;
  }

  // 尝试从其他队列窃取任务（从尾部窃取减少竞争）
  for (size_t i = 0; i < local_queues_.size(); ++i)
  {
    if (i != thread_id && !local_queues_[i].empty())
    {
      // 从尾部窃取任务
      auto& queue = local_queues_[i];
      auto task = std::move(queue.back());
      queue.pop_back();
      return task;
    }
  }

  // 没有可用任务
  return nullptr;
}

// 检查是否有可窃取的任务
bool ThreadPool::has_stealable_tasks() const
{
  std::lock_guard<std::mutex> lock(global_mutex_);
  for (const auto& queue : local_queues_)
  {
    if (!queue.empty())
    {
      return true;
    }
  }
  return false;
}

// 调整线程数量（根据任务负载）
void ThreadPool::adjust_thread_count()
{
  // 只有当任务数超过线程数且未达到最大线程数时才增加线程
  if (total_tasks_ > active_threads_ && threads_.size() < config_.max_threads)
  {
    std::lock_guard<std::mutex> lock(global_mutex_);

    // 再次检查条件（防止竞态条件）
    if (total_tasks_ > active_threads_ && threads_.size() < config_.max_threads)
    {
      size_t new_thread_id = threads_.size();
      try
      {
        threads_.emplace_back(&ThreadPool::worker_thread, this, new_thread_id);
        local_queues_.emplace_back();  // 为新线程创建本地队列
        std::cout << "新增线程 " << new_thread_id
                  << "，当前线程数: " << threads_.size() << std::endl;
      }
      catch (const std::exception& e)
      {
        std::cerr << "创建新线程失败: " << e.what() << std::endl;
      }
    }
  }
}
