/**
 * @file is_same_v.h
 * @author cao zhang
 * @brief 实现一个 is_same_v
 * @version 0.1
 * @date 2025-02-08
 */

#include <iostream>

namespace my
{
  template <typename T1, typename T2>
  constexpr bool is_same_v = false;

  template <typename T>
  constexpr bool is_same_v<T, T> = true;
}  // namespace my

namespace my_test
{
  void test()
  {
    std::cout << std::boolalpha;
    std::cout << my::is_same_v<int, char> << std::endl;
    std::cout << my::is_same_v<double, double> << std::endl;

    std::cout << my::is_same_v<int, int> << std::endl;
  }
}  // namespace my_test