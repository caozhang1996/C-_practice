/**
 * @file template_SFINAE.h
 * @author cao zhang
 * @brief "代换失败不是错误"（Substitution Failure Is Not An Error）示例
 *      在函数模板的重载决议中会应用此规则：当模板行参在替换成显式指定的类型或推导出的类型失败时，从重载集中丢弃这个特化，而非导致编译失败。
 * @version 0.1
 * @date 2025-01-11
 */

#pragma once

#include <iostream>

namespace template_sfinae
{
  template <typename T, typename T2 = typename T::type>
  void func(int)
  {
    std::cout << "int." << std::endl;
  }

  template <typename T>
  void func(double)
  {
    std::cout << "double." << std::endl;
  }

  struct X
  {
    using type = int;
  };

}  // namespace template_sfinae

namespace template_sfinae_test
{
  void test()
  {
    // 编译失败，T 为 int，int::type 不符合基本语法，替换失败，丢弃第一个特化
    template_sfinae::func<int>(1);

    template_sfinae::func<template_sfinae::X>(1);

    // 还是会匹配到第二个特化，因为第一个特化的替换失败了，丢弃了第一个特化
    template_sfinae::func<template_sfinae::X>(1.0);
  }
}  // namespace template_sfinae_test