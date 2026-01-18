/**
 * @file template_class_2.h
 * @author cao zhang
 * @brief 模板模板行参示例
 *        模板模板行参：允许将一个模板作为另一个模板的参数传递。
 * @version 0.1
 * @date 2025-01-15
 */

#pragma once

#include <iostream>
#include <string>

namespace template_class_2
{
  template <typename T>
  struct Y
  {
   public:
    void print() { std::cout << m_value << std::endl; }

    T m_value;
  };

  // T2 是一个模板模板行参，它接受一个类型参数 T1
  template <template <typename T1> typename T2>
  struct X
  {
   public:
    T2<int> m_intMember;
    T2<std::string> m_stringMember;
  };

}  // namespace template_class_2

using namespace template_class_2;

namespace template_class_2_test
{
  void test()
  {
    X<Y> x;
    x.m_intMember.m_value = 42;
    x.m_stringMember.m_value = "Hello, Templates!";

    x.m_intMember.print();     // 输出: 42
    x.m_stringMember.print();  // 输出: Hello, Templates!
  }

}  // namespace template_class_2_test
