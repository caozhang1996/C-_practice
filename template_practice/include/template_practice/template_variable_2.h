/**
 * @file template_variable_2.h
 * @brief 变量模板的偏特化练习
 * @author cao zhang
 * @date 2026-02-08
 * @version 1.0
 */

#include <iostream>

/**
 * @brief 第一种偏特化: 针对某种类型的偏特化
 */
namespace template_variable_2
{
  // 主模板
  template <typename T>
  const char* str = "?";

  // 偏特化，对指针这一“类”类型
  template <typename T>
  const char* str<T*> = "pointer";

  /**
   * 偏特化，但只对 T[] 这一类类型, 而不是数组类型,
   * 因为不同长度的数组都是不同的类型
   */
  template <typename T>
  const char* str<T[]> = "array";
}  // namespace template_variable_2

/**
 * @brief 第二种偏特化: 多个模板类型情况下，对某一个进行特化
 */
namespace template_variable_2
{
  template <typename T1, typename T2>
  const char* str_2 = "?";

  template <typename T2>
  const char* str_2<int, T2> = "T1 is int";

  template <typename T2>
  const char* str_2<char, T2> = "T1 is char";

  template <typename T1>
  const char* str_2<T1, double> = "T2 is double";

}  // namespace template_variable_2

using namespace template_variable_2;

namespace template_variable_2_test
{
  void testOne()
  {
    std::cout << str<int*> << std::endl;
    std::cout << str<double*> << std::endl;
    std::cout << str<void*> << std::endl;
    std::cout << str<char*> << std::endl;

    std::cout << str<int> << std::endl;
    std::cout << str<char> << std::endl;
    std::cout << str<float> << std::endl;

    std::cout << str<int[]> << std::endl;
    std::cout << str<double[]> << std::endl;

    std::cout << str<int[1]> << std::endl;
    std::cout << str<float[1]> << std::endl;
  }

  void testTwo()
  {
    std::cout << str_2<float, float> << std::endl;

    std::cout << str_2<int, float> << std::endl;
    std::cout << str_2<int, char> << std::endl;

    std::cout << str_2<char, float> << std::endl;
    std::cout << str_2<char, int> << std::endl;

    std::cout << str_2<double, double> << std::endl;

    // error: ambiguous template instantiation for ‘str_2<char, double>’
    // std::cout << str_2<char, double> << std::endl;
  }

}  // namespace template_variable_2_test