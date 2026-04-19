/**
 * @file template_tuple.h
 * @brief
 *
 * make_index_sequence 与 tuple 的结合使用：
 *
 * std::make_index_sequence 和 std::index_sequence 是 C++14
 * 引入的模板工具，核心作用：在编译期生成一串连续的数字序列（0,1,2,3...），专门用来遍历
 * 或解包元组、数组、参数包，是模板元编程的神器。
 *
 * std::index_sequence 是一个空的模板类型，只用来携带编译期数字序列。
 * 例如：
 *  std::index_sequence<0,1,2,3> 代表编译期序列：0, 1, 2, 3,
 * 它不占内存、不运行代码，纯编译期工具。
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