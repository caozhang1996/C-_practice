/**
 * @file template_dependent_names_2.h
 * @author cao zhang
 * @brief 待决名 消除歧义符
 * @version 0.1
 * @date 2025-01-18
 */

#pragma once

#include <vector>

// 待决定名（Dependent Names) 消除歧义符

/**
 * 模板定义中不是当前实例化的成员的待决名同样不被认为是模板名，除非使用消除歧义关键词
 * template，或它已经被设立为模板名
 */

namespace template_dependent_names_2
{
  template <typename T>
  struct S
  {
    template <typename U>
    void foo()
    {
    }
  };

  template <typename T>
  void bar()
  {
    /**
     * 错误写法：
     * S<T> s; // s 的类型取决于模板参数 T，因此 s 是一个待决名， 因此
     * s.foo<T>() 中的 foo 不会认为是一个模板名， 因此编译器会报错。
     *
     * s.foo<T>();
     */

    S<T> s;

    // 正确写法：使用 template 关键词消除歧义，告诉编译器 foo
    // 是一个模板名。（不是很常见）
    s.template foo<T>();
  }
};  // namespace template_dependent_names_2

namespace template_dependent_names_2_test
{
  void test() { template_dependent_names_2::bar<int>(); }
};  // namespace template_dependent_names_2_test