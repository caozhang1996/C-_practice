/**
 * @file template_requires_1.h
 * @brief C++ 20 新特性：约束表达式
 *
 * 约束表达式：产生描述约束的 bool 类型的纯右值表达式
 * 约束表达式的语法：
 * requires { 要求序列 }
 * requires ( 行参列表 ( 可选 ) ) { 要求序列 }
 *
 * @author cao zhang
 * @date 2026-03-22
 * @version 1.0
 */

namespace template_requires_1
{
  template <typename T>
  concept Addable = requires(T a, T b)
  {
    a + b;
  };

  template <typename T, typename U>
  concept Swappable = requires(T t, U u)
  {
    swap(t, u);
    swap(u, t);
  };

  struct X
  {
    int value_;

    X(int value) : value_(value) {}

    X operator+(const X& other) const { return X(this->value_ + other.value_); }
  };

  void swap(X& x1, X& x2)
  {
    int temp_value;
    temp_value = x1.value_;
    x1.value_ = x2.value_;
    x2.value_ = temp_value;
  }

  template <typename T>
  requires(Addable<T>&& Swappable<T, T>) struct Test
  {
    T t_;

    Test(const T& t) : t_(t) {}

    void modifyMember(const T& t) { t_ = t_ + t; }

    void printMember() { std::cout << t_.value_ << std::endl; }
  };

}  // namespace template_requires_1

namespace template_requires_1_test
{
  void test()
  {
    std::cout
        << std::boolalpha
        << template_requires_1::Addable<template_requires_1::X> << std::endl;

    std::cout
        << std::boolalpha
        << template_requires_1::Swappable<template_requires_1::X,
                                          template_requires_1::X> << std::endl;

    template_requires_1::Test<template_requires_1::X> t(
        template_requires_1::X{10});

    t.modifyMember(template_requires_1::X{2});

    t.printMember();
  }
}  // namespace template_requires_1_test