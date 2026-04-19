/**
 * @file template_if.h
 * @brief
 *
 * C++ 17 引入的编译期 if
 *
 * @author cao zhang
 * @date 2026-04-19
 * @version 1.0
 */

#include <iostream>
#include <type_traits>

namespace template_if
{
  struct X
  {
    void fun() const { std::cout << "X::func" << std::endl; }
  };

  struct Y
  {
    void f() const { std::cout << "Y::f" << std::endl; }
  };

  template <typename T>
  void boo(T t)
  {
    if constexpr (std::is_same_v<T, X>)
    {
      t.fun();
    }
    else
    {
      t.f();
    }
  }
}  // namespace template_if

namespace template_if_test
{
  void test()
  {
    template_if::X x;
    template_if::Y y;
    template_if::boo(x);
    template_if::boo(y);
  }
}  // namespace template_if_test