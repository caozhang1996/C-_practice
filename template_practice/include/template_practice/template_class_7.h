/**
 * @file template_class_7.h
 * @author cao zhang
 * @brief 类模板的偏特化练习
 * @version 0.1
 * @date 2025-01-31
 */

#include <iostream>

namespace template_class_7
{
  template <typename T1, typename T2>
  struct X
  {
    void func() const;
  };

  // 类内声明，类外定义
  template <typename T1, typename T2>
  void X<T1, T2>::func() const
  {
    std::cout << "main template" << std::endl;
  }

  template <typename T2>
  struct X<int, T2>
  {
    void func() const;

    void boo() const;
  };

  // 类内声明，类外定义
  template <typename T2>
  void X<int, T2>::func() const
  {
    std::cout << "T1 is int" << std::endl;
  }

  template <typename T2>
  void X<int, T2>::boo() const
  {
    std::cout << "boo" << std::endl;
  }

  template <typename T2>
  struct X<char, T2>
  {
    void func() const { std::cout << "T1 is char" << std::endl; }
  };

  template <typename T1>
  struct X<T1, int>
  {
    void func() const { std::cout << "T2 is char" << std::endl; }
  };

}  // namespace template_class_7

namespace template_class_7_test
{
  void testOne()
  {
    template_class_7::X<double, double> x1;
    x1.func();

    template_class_7::X<int, double> x2;
    x2.func();
    x2.boo();

    template_class_7::X<char, double> x3;
    x3.func();

    template_class_7::X<float, int> x4;
    x4.func();
  }
}  // namespace template_class_7_test
