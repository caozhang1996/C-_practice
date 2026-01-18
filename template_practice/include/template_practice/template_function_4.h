/**
 * @file template_function_4.h
 * @author cao zhang
 * @brief 函数模板全特化示例
 * @version 0.1
 * @date 2025-01-18
 */

#pragma once

#include <iostream>

namespace template_function_4
{
  template <typename T1, typename T2>
  auto func(const T1& a, const T2& b)
  {
    return a + b;
  }

  // 全特化版本，当 a 为 int 类型且 b 为 double 类型时，调用此定制化的相减版本
  template <>
  auto func(const int& a, const double& b)
  {
    return a - b;
  }
};  // namespace template_function_4

using namespace template_function_4;

namespace template_function_4_test
{
  void test()
  {
    std::cout << "func(10, 20): " << func(10, 20)
              << std::endl;  // 调用通用版本，输出30
    std::cout << "func(10.5, 20.5): " << func(10.5, 20.5)
              << std::endl;  // 调用通用版本，输出31
    std::cout << "func(10, 20.5): " << func(10, 20.5)
              << std::endl;  // 调用全特化版本，输出-10.5
    std::cout << "func(15, 5.5): " << func(15, 5.5)
              << std::endl;  // 调用全特化版本，输出9.5
  }

};  // namespace template_function_4_test