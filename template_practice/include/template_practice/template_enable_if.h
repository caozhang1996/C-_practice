/**
 * @file template_dependent_names_1.h
 * @author cao zhang
 * @brief 标准库 enable_if 的使用
 * @version 0.1
 * @date 2026-03-15
 */

#pragma once

#include <type_traits>

namespace template_enable_if_test
{
  // 要求函数的传参必须是 int 类型的
  template <typename T,
            typename type = std::enable_if_t<std::is_same_v<T, int>>>
  void foo(T)
  {
  }

  void test()
  {
    foo(1);

    // 编译失败
    // foo(1.0);
  }
}  // namespace template_enable_if_test