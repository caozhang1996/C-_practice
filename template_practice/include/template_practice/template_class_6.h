/**
 * @file template_class_6.h
 * @author cao zhang
 * @brief 对类模板的成员函数模板进行特化
 * @version 0.1
 * @date 2025-01-31
 */

#include <iostream>
#include <type_traits>

namespace template_class_6
{
  // 注意这里的类 X 是模板类
  template <typename T>
  struct X
  {
    template <typename U>
    void func(U u)
    {
      std::cout << "class type: " << typeid(T).name()
                << ", param type: " << typeid(U).name() << ", value: " << u
                << std::endl;
    }
  };

  template <>
  template <>
  void X<char>::func<int>(int u)
  {
    std::cout << "char class, int type, value: " << u << std::endl;
  }
}  // namespace template_class_6

namespace template_class_6_test
{
  void test()
  {
    template_class_6::X<char> x1;
    x1.func(1.23);
    x1.func(10);

    template_class_6::X<int> x2;
    x2.func('a');
    x2.func(10);
  }
};  // namespace template_class_6_test