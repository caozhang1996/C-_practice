/**
 * @file template_class_3.h
 * @author cao zhang
 * @brief 类模板的全特化示例
 * @version 0.1
 * @date 2025-01-12
 */

#pragma once

#include <iostream>

namespace template_class_3
{
  template <typename T>
  struct is_void
  {
    static constexpr bool value = false;
  };

  template <>
  struct is_void<void>
  {
    static constexpr bool value = true;
  };

  template <typename T>
  constexpr bool is_void_v = is_void<T>::value;

}  // namespace template_class_3

using namespace template_class_3;

namespace template_class_3_test
{
  void test()
  {
    std::cout << std::boolalpha;

    // tips: 这里的 value 静态数据成员属于实际的类，而不是类模板
    // 模板实例化不同的类，那么它们的静态数据成员也不是同一个
    std::cout << is_void<void>::value << std::endl;
    std::cout << is_void<int>::value << std::endl;
    std::cout << is_void<double>::value << std::endl;

    std::cout << is_void_v<std::string> << std::endl;
  }

}  // namespace template_class_3_test