#include <chrono>
#include <iostream>
#include <thread>

// 线程函数：接收 stop_token 以检查中断
void long_running_task(std::stop_token st)
{
  // 注册中断回调
  // 其底层原理是构造 std::stop_callback 对象时，中断回调函数注册到 st 内部
  std::stop_callback cb(st, []() { std::cout << "中断回调：执行资源清理\n"; });

  for (int i = 0; i < 10; ++i)
  {
    // 检查是否收到中断请求
    if (st.stop_requested())
    {
      std::cout << "任务收到中断请求，退出\n";
      return;
    }
    std::cout << "任务执行中：" << i << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时操作
  }
  std::cout << "任务正常完成\n";
}

int main()
{
  // 创建 jthread，线程函数会自动接收 stop_token（需函数第一个参数为
  // stop_token）
  std::jthread t(long_running_task);

  // 主线程等待 3 秒后发送中断请求
  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::cout << "主线程发送中断请求\n";

  // 获取 jthread 的 stop_source 并发送中断
  t.request_stop();

  // jthread 析构时自动 join（无需手动调用）
  return 0;
}