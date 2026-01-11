
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

namespace alignas_learning
{
#define LOOP_NUM 1e9

  // 示例结构体：没有缓存对齐
  struct NonAligned
  {
    std::atomic<uint32_t> x;
    std::atomic<uint32_t> y;
  };

  // 示例结构体：有缓存对齐
  struct Aligned
  {
    alignas(64) std::atomic<uint32_t> x;
    alignas(64) std::atomic<uint32_t> y;
  };

  // 测试函数：没有缓存对齐
  void non_aligned_test_x(NonAligned& _data)
  {
    for (size_t i = 0; i < LOOP_NUM; ++i)
    {
      ++_data.x;
    }
  }

  void non_aligned_test_y(NonAligned& _data)
  {
    for (size_t i = 0; i < LOOP_NUM; ++i)
    {
      ++_data.y;
    }
  }

  // 测试函数：有缓存对齐
  void aligned_test_x(Aligned& _data)
  {
    for (size_t i = 0; i < LOOP_NUM; ++i)
    {
      ++_data.x;
    }
  }

  void aligned_test_y(Aligned& _data)
  {
    for (size_t i = 0; i < LOOP_NUM; ++i)
    {
      ++_data.y;
    }
  }

  void test()
  {
    // 没有缓存对齐的测试
    {
      NonAligned data;
      data.x = 0;
      data.y = 0;

      // 打印变量地址
      std::cout << "NonAligned x address: " << &data.x << std::endl;
      std::cout << "NonAligned y address: " << &data.y << std::endl;
      std::cout << "Address difference (NonAligned): "
                << (reinterpret_cast<char*>(&data.y) -
                    reinterpret_cast<char*>(&data.x))
                << " bytes" << std::endl;

      auto start = std::chrono::high_resolution_clock::now();
      std::thread t1(non_aligned_test_x, std::ref(data));
      std::thread t2(non_aligned_test_y, std::ref(data));
      t1.join();
      t2.join();
      auto end = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> duration = end - start;
      std::cout << "Non-Aligned Duration: " << duration.count() << " seconds"
                << std::endl;
    }

    // 有缓存对齐的测试
    {
      Aligned data;
      data.x = 0;
      data.y = 0;

      // 打印变量地址
      std::cout << "Aligned x address: " << &data.x << std::endl;
      std::cout << "Aligned y address: " << &data.y << std::endl;
      std::cout << "Address difference (Aligned): "
                << (reinterpret_cast<char*>(&data.y) -
                    reinterpret_cast<char*>(&data.x))
                << " bytes" << std::endl;

      auto start = std::chrono::high_resolution_clock::now();
      std::thread t1(aligned_test_x, std::ref(data));
      std::thread t2(aligned_test_y, std::ref(data));
      t1.join();
      t2.join();
      auto end = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> duration = end - start;
      std::cout << "Aligned Duration: " << duration.count() << " seconds"
                << std::endl;
    }
  }
}  // namespace alignas_learning

int main()
{
  alignas_learning::test();
  return 0;
}

// #include <iostream>
// #include <memory>

// class MyClass : public std::enable_shared_from_this<MyClass>
// {
//  public:
//   void do_something()
//   {
//     // 获取当前对象的 shared_ptr
//     std::shared_ptr<MyClass> self = shared_from_this();
//     std::cout << "Use count: " << self.use_count() << std::endl;
//   }
// };

// int main()
// {
//   std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
//   obj->do_something();  // 输出: Use count: 2
// }
