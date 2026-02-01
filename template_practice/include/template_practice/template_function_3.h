/**
 * @file template_function_3.h
 * @author cao zhang
 * @brief 可变参数模板函数示例
 * @version 0.1
 * @date 2025-01-11
 */

#pragma once

#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>

using namespace std::string_literals;

namespace template_function_3
{
  template <typename T>
  void printOne(const T& t)
  {
    std::cout << t << std::endl;
  }

  template <typename T, typename... Args>
  void printOne(const T& t, const Args&... args)
  {
    std::cout << t << " ";
    printOne(args...);
  }

  // 打印可变参数，每个参数之间用空格分隔，最后一个元素没有多余空格
  template <typename... Args>
  void printAll(const Args&... args)
  {
    printOne(args...);
    std::cout << sizeof...(args) << " elements printed."
              << std::endl;  // sizeof... 获取参数个数
  }

  // 另一种实现方式，使用 fold 表达式
  template <typename... Args>
  void printAllFold(const Args&... args)
  {
    ((std::cout << args << " "), ...);  // 使用 fold 表达式和逗号运算符
    std::cout << std::endl;
    std::cout << sizeof...(args) << " elements printed."
              << std::endl;  // sizeof... 获取参数个数
  }

  // 写一个函数 sum，支持任意类型和数量的参数求和（这里的类型是 int、char、float
  // 等可被加的类型）
  template <typename... Args, typename RT = std::common_type_t<Args...>>
  RT sum(const Args&... args)
  {
    RT nums[] = {args...};

    // RT{} 这里不能是 0，否则转换为 int 类型，会丢失精度, 而应该是 RT 的默认值
    return std::accumulate(std::begin(nums), std::end(nums), RT{});
  }

}  // namespace template_function_3

using namespace template_function_3;

namespace template_function_3_test
{
  void testPrintAll()
  {
    printAll(1, 2, 3.14, "hello"s);
    printAllFold(1, 2, 3.14, "hello"s);

    std::cout << "Sum: " << sum(1, 2, 3.5, 4.0f) << std::endl;  // 输出 10.5
  }

}  // namespace template_function_3_test
