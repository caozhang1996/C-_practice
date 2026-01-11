/**
 * @file template_function_2.h
 * @author cao zhang
 * @brief 模板函数重载示例
 * @version 0.1
 * @date 2025-01-11
 */

#pragma once

#include <iostream>
#include <type_traits>

namespace template_function_2
{
  template <typename T>
  void process(const T& value)
  {
    std::cout << "Template version, processing value: " << value << std::endl;
  }

  void process(int value)
  {
    std::cout << "Int version, Processing integral: " << value << std::endl;
  }

  //   // 重载版本 1: 处理整数类型
  //   template <typename T>
  //   typename std::enable_if<std::is_integral<T>::value, void>::type process(
  //       const T& value)
  //   {
  //     std::cout << "Processing integral: " << value << std::endl;
  //   }

  //   // 重载版本 2: 处理浮点类型
  //   template <typename T>
  //   typename std::enable_if<std::is_floating_point<T>::value, void>::type
  //   process(
  //       const T& value)
  //   {
  //     std::cout << "Processing floating point: " << value << std::endl;
  //   }

}  // namespace template_function_2

using namespace template_function_2;

namespace template_function_2_test
{
  void test_process()
  {
    process(42);       // 调用整数版本
    process(3.14);     // 调用通用模板版本
    process("test"s);  // 调用通用模板版本
  }
}  // namespace template_function_2_test
