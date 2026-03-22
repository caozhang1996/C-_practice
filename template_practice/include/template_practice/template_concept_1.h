/**
 * @file template_concept_1.h
 * @brief C++ 20 新特性：约束与概念
 * @author cao zhang
 * @date 2026-03-22
 * @version 1.0
 */

#include <type_traits>

namespace template_concept_1
{
  // 我需要写一个函数模板 add。 想要要求传入的对象必须是支持 operator+
  // 的，应该怎么写？（使用 C++ 的约束与概念）

  /**Intergral
   * concept 语法
   * tempalte <模板行参列表>
   * concept 概念名 属性（可选） = 约束表达式;
   */

  /**
   * 下列概念的语义是：约束模板类型行参 T，即要求 T 必须满足约束表达式的要求序列
   * a + a, 如果 add 模板函数中的 T 类型不满足，则不会选择这个模板函数 T
   */

  /**
   * 定义概念（concept）时声明的约束表达式，只需要是编译期可得到 bool
   * 类型的表达式即可。
   * 例如：
   *  template<typename T>
   *  concept intergral = std::is_intergral_v<T>;
   *
   *  template<typename T>
   *  concept Int = std::is_same_v<T, int>;
   *
   * 下述的 requires 表达式，也是会返回 bool 值的，简单来说，把模板参数带入到
   * requires 表达式中，是否符合语法，符合就返回 true，不符合就返回 false
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

  // 合取：需要满足多个条件的约束
  template <typename T>
  concept Intergral = std::is_integral_v<T>;

  template <typename T>
  concept SignedIntergral = Intergral<T> && std::is_signed_v<T>;

  void fun(const SignedIntergral auto& x) {}

}  // namespace template_concept_1

namespace template_concept_1_test
{
  void test()
  {
    struct X
    {
      int value_;

      X(int value) : value_(value) {}

      X operator+(const X& other) const { return this->value_ + other.value_; }
    };

    template_concept_1::add(X{1}, X{2});
    template_concept_1::add(1, 2);

    template_concept_1::fun(1);
    // template_concept_1::fun(1u); // 编译报错
  }
}  // namespace template_concept_1_test