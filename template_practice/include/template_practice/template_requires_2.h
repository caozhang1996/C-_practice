/**
 * @file template_requires_1.h
 * @brief C++ 20 新特性：约束表达式
 *
 * 类型要求：要求模板参数包含特有的类型
 *
 * @author cao zhang
 * @date 2026-03-22
 * @version 1.0
 */

#include <iostream>

namespace template_requires_2
{
  template <typename T>
  concept C = requires
  {
    typename T::Type;  // 要求 T 类型中包含有 Type 类
    typename T::type;  // 要求 T 类型中包含有 type 类
  };

  struct X1
  {
    using Type = int;
    using type = char;
  };

  struct X2
  {
    struct Type
    {
    };
  };

}  // namespace template_requires_2

namespace template_requires_2_test
{
  void test()
  {
    std::cout << std::boolalpha
              << template_requires_2::C<template_requires_2::X1> << std::endl;
    std::cout << std::boolalpha
              << template_requires_2::C<template_requires_2::X2> << std::endl;
  }
}  // namespace template_requires_2_test