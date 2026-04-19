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
  // 要求函数的传参必须是 int 类型的， 在该示例中没有给 std::enable_if_t
  // 传入第二个参数，则 type 的默认类型为 void
  template <typename T,
            typename type = std::enable_if_t<std::is_same_v<T, int>>>
  void foo(T)
  {
  }

  // 第二种写法，给 std::enable_if_t 传入第二个参数
  // 此时 int = 0 (无名参数)
  template <typename T, std::enable_if_t<std::is_same_v<T, double>, int> = 0>
  void boo(T)
  {
  }

  // bool = true (无名参数)
  template <typename T,
            std::enable_if_t<std::is_same_v<T, double>, bool> = true>
  void boo_1(T)
  {
  }

  // int* = nullptr (无名参数)
  template <typename T,
            std::enable_if_t<std::is_same_v<T, double>, int*> = nullptr>
  void boo_2(T)
  {
  }

  void test()
  {
    foo(1);

    boo(1.0);

    // 编译失败
    // foo(1.0);
  }
}  // namespace template_enable_if_test