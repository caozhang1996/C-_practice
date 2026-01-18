/**
 * @file template_class_1.h
 * @author cao zhang
 * @brief 用户定义推导指引示例
 * @version 0.1
 * @date 2025-01-12
 */

#pragma once

#include <iostream>

namespace template_class_1
{
  template <typename Ty, std::size_t N>
  class array
  {
   public:
    template <typename... Args>
    array(Args... args) : m_data{args...}
    {
    }

    void print()
    {
      for (const auto& item : m_data)
      {
        std::cout << item << " ";
      }
      std::cout << std::endl;
    }

   private:
    Ty m_data[N];
  };

  // 用户定义推导指引, 用于从构造函数参数推导出模板参数
  template <typename Ty, typename... Args>
  array(Ty, Args...) -> array<Ty, 1 + sizeof...(Args)>;
}  // namespace template_class_1

using namespace template_class_1;

namespace template_class_1_test
{
  void test()
  {
    /**
     * array arr(1, 2, 3, 4); 这样的方式构造 array
        时，编译器会自动推导出模板参数类型和大小。

        具体来说：

        Ty 会被推导为第一个参数的类型（如 int）。
        N 会被推导为参数个数（如 4）。
        所以 array arr(1, 2, 3, 4); 实际等价于 array<int, 4> arr{1, 2, 3, 4};
        没有这个推导指引时，必须写成 array<int, 4> arr{1, 2, 3,
        4};，否则编译器无法自动推导出 N。
     */
    array arr1{1, 2, 3, 4};

    arr1.print();  // 输出: 1 2 3 4
  }
}  // namespace template_class_1_test