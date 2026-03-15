/**
 * @file template_void_t.h
 * @author cao zhang
 * @brief
 *
 * std::void_t 的标准库实现如下：
 *
 * template<typename ...>
 * using viod_t = void;
 *
 * 它的实现非常非常的简单，就是一个别名，接受任意个数的类型参数，但自身始终是
 * void 类型。
 * 1、将任意类型的序列映射到类型 void 的工具元函数。
 * 2、模板元编程中，用此元函数检测 SFINAE 语境中的非良构类型。
 *
 * @version 0.1
 * @date 2026-03-15
 */

#pragma once

#include <type_traits>

namespace template_void_t_test
{
  // 实现一个 add 函数，要求传入的对象需要支持 + 以及它需要有别名 type， 成员
  // value 和 f
  template <typename T,
            typename type =
                std::void_t<decltype(std::declval<T>() + std::declval<T>()),
                            typename T::type,
                            decltype(&T::value),
                            decltype(&T::f)>>
  auto add(const T& t1, const T& t2)
  {
    return t1 + t2;
  }

  struct X
  {
    X(int v) : value(v) {}

    int value;
    using type = void;

    int operator+(const X& x) const { return value + x.value; }
    void f() {}
  };

  void test()
  {
    X x(1);
    add(x, x);
  }
}  // namespace template_void_t_test