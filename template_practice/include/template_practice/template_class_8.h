/**
 * @file template_class_8.h
 * @author cao zhang
 * @brief 显示实例化分 h 和 cpp 文件示例
 * @version 0.1
 * @date 2025-01-31
 */

#pragma once
#include <iostream>

namespace template_class_8
{
  template <typename T>
  struct X
  {
    void func1();

    void func2();
  };
}  // namespace template_class_8

namespace template_class_8_test
{
  inline void test()
  {
    template_class_8::X<int> x1;
    x1.func1();
    x1.func2();

    template_class_8::X<char> x2;
    x2.func1();
    x2.func2();
  }
}  // namespace template_class_8_test