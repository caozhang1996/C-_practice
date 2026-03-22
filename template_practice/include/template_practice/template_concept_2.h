/**
 * @file template_concept_2.h
 * @brief C++ 20 新特性：约束与概念
 * @author cao zhang
 * @date 2026-03-22
 * @version 1.0
 */

namespace template_concept_2
{
  struct X
  {
    int value_;
  };

  // 编译期常量表达式
  template <typename T>
  constexpr bool get_value()
  {
    return true;
  }

  // 合取：要求 T 它的大小 sizeof 大于 1,并且支持一个 get_value 的函数调用
  template <typename T>
  requires(sizeof(T) > 1 && get_value<T>()) void fun(T t) {}
}  // namespace template_concept_2

namespace template_concept_2_test
{
  void test()
  {
    // using namespace template_concept_2;

    template_concept_2::fun(template_concept_2::X{});
  }
}  // namespace template_concept_2_test