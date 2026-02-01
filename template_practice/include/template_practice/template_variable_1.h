/**
 * @file template_variable_1.h
 * @brief 变量模板练习1
 * @author cao zhang
 * @date 2026-01-18
 * @version 1.0
 */

#pragma once

#include <iostream>
#include <type_traits>

namespace template_variable_1
{
  // 模板参数为类型
  template <typename T>
  constexpr T v1 = 10;

  // 模板参数为非类型
  template <std::size_t N = 10>
  constexpr std::size_t v2 = N;

  // 模板参数为可变非类型参数
  template <std::size_t... values>
  constexpr std::size_t aa[] = {values...};

  template <typename T>
  bool b = std::is_integral_v<T>;

}  // namespace template_variable_1

// 变量模板全特化
namespace template_variable_1
{
  template <typename T>
  constexpr const char* s = "??";

  // void 类型全特化
  template <>
  constexpr const char* s<void> = "void";

  // char 类型全特化
  template <>
  constexpr const char* s<char> = "char";

  template <typename T>
  constexpr const bool is_void_v = false;

  template <>
  constexpr const bool is_void_v<void> = true;
}  // namespace template_variable_1

using namespace template_variable_1;

namespace template_variable_1_test
{
  void test()
  {
    std::cout << "v1<int>: " << v1<int> << std::endl;
    std::cout << "v1<double>: " << v1<double> << std::endl;
    std::cout << "v1<float>: " << v1<float> << std::endl;

    std::cout << "v2<>: " << v2<> << std::endl;
    std::cout << "v2<20>: " << v2<20> << std::endl;
    std::cout << "v2<50>: " << v2<50> << std::endl;

    for (std::size_t v : aa<1, 2, 3, 4, 5>)
    {
      std::cout << v << " ";
    }

    std::cout << std::endl;
    std::cout << std::boolalpha
              << std::is_same_v<decltype(aa<1, 2, 3, 4, 5>),
                                const std::size_t[5]> << std::endl;

    std::cout << std::boolalpha << b<double> << std::endl;
    std::cout << std::boolalpha << b<double> << std::endl;
    std::cout << std::boolalpha << b<int> << std::endl;
  }

  void testFullySpecialized()
  {
    std::cout << "s<int>: " << s<int> << std::endl;
    std::cout << "s<void>: " << s<void> << std::endl;
    std::cout << "s<char>: " << s<char> << std::endl;

    std::cout << "is_void_v<int>: "
              << template_variable_1::is_void_v<int> << std::endl;
    std::cout << "is_void_v<void>: "
              << template_variable_1::is_void_v<void> << std::endl;
  }
}  // namespace template_variable_1_test