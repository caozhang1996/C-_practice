/**
 * @file template_class_5.h
 * @author cao zhang
 * @brief 对类的成员函数模板进行特化
 * @version 0.1
 * @date 2025-01-31
 */

#include <iostream>
#include <type_traits>

namespace template_class_5
{
  // 注意这里的类 X 不是模板类
  struct X
  {
    template <typename T>
    void func(T t)
    {
      std::cout << "type: " << typeid(T).name() << ", value: " << t
                << std::endl;
    }
  };

  template <>
  void X::func<int>(int t)
  {
    std::cout << "int type, value: " << t << std::endl;
  }
}  // namespace template_class_5

namespace template_class_5_test
{
  void test()
  {
    template_class_5::X x;
    x.func(1.23);
    x.func("hello world!");
    x.func(10);
  }
};  // namespace template_class_5_test