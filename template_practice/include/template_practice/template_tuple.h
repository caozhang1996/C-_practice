/**
 * @file template_tuple.h
 * @brief
 *
 * make_index_sequence 与 tuple 的结合使用
 *
 * @author cao zhang
 * @date 2026-03-22
 * @version 1.0
 */

#include <iostream>
#include <tuple>

namespace template_tuple
{
  // 通用的打印 tuple 的函数
  template <typename... Args>
  void printTuple(const std::tuple<Args...>& tuple)
  {
    auto printElement = [&]<std::size_t... I>(std::index_sequence<I...>)
    {
      ((std::cout << std::get<I>(tuple) << " "), ...);
    };

    printElement(std::make_index_sequence<sizeof...(Args)>{});
  }
}  // namespace template_tuple

namespace template_tuple_test
{
  void test()
  {
    auto myTuple = std::make_tuple(42, 3.14, "Hello, World!");
    template_tuple::printTuple(myTuple);
  }
}  // namespace template_tuple_test