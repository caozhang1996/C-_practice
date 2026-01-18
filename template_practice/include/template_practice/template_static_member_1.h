/**
 * @file template_static_member_1.h
 * @author cao zhang
 * @brief 类的静态数据成员是模板
 * @version 0.1
 * @date 2025-01-18
 */

#pragma once

#include <iostream>

namespace template_static_member_1
{
  class MyClass11
  {
   public:
    template <typename T>
    static const T value;  // 静态数据成员模板的声明，不是定义！！！
  };

  template <typename T>
  const T MyClass11::value{};  // 静态数据成员模板的定义！！！

  // C++17 的写法：可以将声明和定义合并在一起，使用 inline 关键字
  class MyClassCpp17
  {
   public:
    template <typename T>
    inline static const T value{};
  };
};  // namespace template_static_member_1

using namespace template_static_member_1;

namespace template_static_member_1_test
{
  void test()
  {
    std::cout << MyClass11::value<int> << std::endl;
    std::cout << MyClass11::value<double> << std::endl;
    std::cout << MyClass11::value<float> << std::endl;

    std::cout << MyClassCpp17::value<int> << std::endl;
    std::cout << MyClassCpp17::value<double> << std::endl;
    std::cout << MyClassCpp17::value<float> << std::endl;
  }
}  // namespace template_static_member_1_test