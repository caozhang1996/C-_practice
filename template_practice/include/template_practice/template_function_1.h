/**
 * @file template_function.h
 * @author cao zhang
 * @brief 简单的模板函数示例
 * @version 0.1
 * @date 2025-01-11
 */

#pragma once

#include <iostream>
#include <string>

using namespace std::string_literals;

namespace template_function_1
{
  template <typename T>
  T max(const T& a, const T& b)
  {
    return a > b ? a : b;
  }

  // 返回"公共类型"的模板函数
  template <typename T1,
            typename T2,
            typename RT = decltype(true ? std::declval<T1>()
                                        : std::declval<T2>())>
  RT max2(const T1& a, const T2& b)
  {
    return a > b ? a : b;
  }

  // 带默认类型参数的模板函数
  template <typename T = int>
  void print(const T& value)
  {
    std::cout << value << std::endl;
  }

}  // namespace template_function_1

using namespace template_function_1;

namespace template_function_1_test
{
  void testMax()
  {
    std::cout << max(1, 2) << std::endl;
    std::cout << max<double>(3.0, 1) << std::endl;  // 没有推导，显示指明了类型

    // Error: 无法确认 T 类型到底是 const char* 还是 std::string
    // std::cout << max("hello"s, "1") << std::endl;

    std::cout << std::max("hello"s, "1"s)
              << std::endl;  // 使用 std 命名空间下的 max 函数

    std::cout << max2(1, 2.5) << std::endl;  // RT 被推导为 double
  }

  void testPrint()
  {
    print(100);                            // 使用默认类型参数 int
    print(3.14);                           // T 被推导为 double
    print<std::string>("Hello, World!"s);  // 显示指明 T 为 std::string
  }

}  // namespace template_function_1_test
