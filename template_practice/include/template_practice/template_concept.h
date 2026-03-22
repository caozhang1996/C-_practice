/**
 * @file template_concept.h
 * @brief C++ 20 新特性：约束与概念
 * @author cao zhang
 * @date 2026-03-22
 * @version 1.0
 */

namespace template_concept
{
  // 我需要写一个函数模板 add。 想要要求传入的对象必须是支持 operator+
  // 的，应该怎么写？（使用 C++ 的约束与概念）

  /**
   * concept 语法
   * tempalte <模板行参列表>
   * concept 概念名 属性（可选） = 约束表达式;
   */

  /**
   * 下列概念的语义是：约束模板类型行参 T，即要求 T 必须满足约束表达式的要求序列
   * a + a, 如果 add 模板函数中的 T 类型不满足，则不会选择这个模板函数 T
   */
  template <typename T>
  concept Add = requires(T a)
  {
    a + a;
  };

  template <Add T>
  auto add(const T& t1, const T& t2)
  {
    return t1 + t2;
  }

}  // namespace template_concept

namespace template_concept_test
{
  void test()
  {
    struct X
    {
      int value_;

      X(int value) : value_(value) {}

      X operator+(const X& other) const { return this->value_ + other.value_; }
    };

    template_concept::add(X{1}, X{2});
    template_concept::add(1, 2);
  }
}  // namespace template_concept_test