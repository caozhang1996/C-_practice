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

}  // namespace template_variable_1

using namespace template_variable_1;

namespace template_variable_1_test
{
  void test()
  {
    std::cout << "pi<int>: " << v1<int> << std::endl;
    std::cout << "pi<double>: " << v1<double> << std::endl;
    std::cout << "pi<float>: " << v1<float> << std::endl;

    std::cout << "v<>: " << v2<> << std::endl;
    std::cout << "v<20>: " << v2<20> << std::endl;
    std::cout << "v<50>: " << v2<50> << std::endl;

    for (std::size_t v : aa<1, 2, 3, 4, 5>)
    {
      std::cout << v << " ";
    }

    std::cout << std::endl;
    std::cout << std::boolalpha
              << std::is_same_v<decltype(aa<1, 2, 3, 4, 5>),
                                const std::size_t[5]> << std::endl;
  }
}  // namespace template_variable_1_test