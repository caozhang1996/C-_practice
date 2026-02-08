#include "template_practice/template_class_8.h"

namespace template_class_8
{
  template <typename T>
  void X<T>::func1()
  {
    std::cout << "func1" << std::endl;
  }

  template <typename T>
  void X<T>::func2()
  {
    std::cout << "func2" << std::endl;
  }

  // 实例化 int 类型的全部成员函数,这样不用一个一个写成员函数的实现了
  template struct X<int>;

  // 实例化 char 类型的全部成员函数,这样不用一个一个写成员函数的实现了
  template struct X<char>;
}  // namespace template_class_8