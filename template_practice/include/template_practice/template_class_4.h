/**
 * @file template_class_4.h
 * @author cao zhang
 * @brief 对类模板内的成员类或者成员类模板进行特化
 * @version 0.1
 * @date 2025-01-31
 */

#include <iostream>

namespace template_class_4
{
  template <typename T>
  struct A
  {
    // 成员类
    struct B
    {
    };

    // 成员类模板
    template <typename U>
    struct C
    {
    };
  };

  // 特化 A 的 void 类型
  template <>
  struct A<void>
  {
    void func();
  };

  // 在特化的 A<void> 类型外定义 func 函数
  void A<void>::func() { std::cout << "hello world!" << std::endl; }

  // 特化 A<char> 情况下的 B 类的定义
  template <>
  struct A<char>::B
  {
    void func();
  };

  void A<char>::B::func() { std::cout << "yes" << std::endl; };

  // 特化 A<int> 的模板类 C 的定义
  template <>
  template <typename U>
  struct A<int>::C
  {
    void func();
  };

  template <>
  template <typename U>
  void A<int>::C<U>::func()
  {
    std::cout << "good" << std::endl;
  };
}  // namespace template_class_4

using namespace template_class_4;

namespace template_class_4_test
{
  void test()
  {
    A<void> aVoid;
    aVoid.func();

    A<char>::B aCharB;
    aCharB.func();

    // 不管 C 实例化什么类型，都是使用同一个 func 函数
    A<int>::C<void> aIntCVoid;
    A<int>::C<int> aIntCInt;
    aIntCVoid.func();
    aIntCInt.func();
  }
}  // namespace template_class_4_test