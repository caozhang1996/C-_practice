#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "practice/thread_pool.h"

// 测试任务：计算1到n的和
int sum_task(int n)
{
  int sum = 0;
  for (int i = 1; i <= n; ++i)
  {
    sum += i;
  }
  return sum;
}

// 测试任务：模拟耗时操作
void delay_task(int ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main()
{
  try
  {
    // 创建线程池配置
    ThreadPoolConfig config;
    config.min_threads = 2;
    config.max_threads = 8;
    config.enable_affinity = true;  // 启用线程亲和性（测试用）

    // 创建线程池
    ThreadPool pool(config);

    std::cout << "线程池测试开始" << std::endl;

    // 测试1：提交一批计算任务
    std::vector<std::future<int>> sum_futures;
    for (int i = 1; i <= 10; ++i)
    {
      sum_futures.push_back(pool.submit(sum_task, i * 1000));
    }

    // 等待结果并输出
    std::cout << "\n计算任务结果：" << std::endl;
    for (auto& fut : sum_futures)
    {
      std::cout << "结果: " << fut.get() << std::endl;
    }

    // 测试2：提交一批延迟任务，观察线程动态调整
    std::cout << "\n提交延迟任务，观察线程池动态调整..." << std::endl;
    std::vector<std::future<void>> delay_futures;
    for (int i = 0; i < 20; ++i)
    {
      delay_futures.push_back(pool.submit(delay_task, 100));
    }

    // 等待延迟任务完成
    for (auto& fut : delay_futures)
    {
      fut.get();
    }

    // 测试3：测试异常处理
    std::cout << "\n测试异常处理..." << std::endl;
    auto bad_future =
        pool.submit([]() { throw std::runtime_error("这是一个测试异常"); });

    try
    {
      bad_future.get();
    }
    catch (const std::exception& e)
    {
      std::cout << "捕获到预期异常: " << e.what() << std::endl;
    }

    std::cout << "\n所有测试完成" << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cerr << "测试失败: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
