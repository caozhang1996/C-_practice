/**
 * @file template_SFINAE.h
 * @author cao zhang
 * @brief SFINAE（Substitution Failure Is Not An Error）基础使用示例
 * @version 0.1
 * @date 2025-01-11
 */

#pragma once

#include <iostream>
#include <type_traits>

namespace template_sfinae_example
{
  // 写一个函数模板 add，要求传入的对象必须是支持 operator+ 的
  template <typename T, typename = void>
  struct has_add_operator : std::false_type
  {
  };

  template <typename T>
  struct has_add_operator<
      T,
      std::void_t<decltype(std::declval<T>() + std::declval<T>())>>
      : std::true_type
  {
  };

  template <typename T>
  typename std::enable_if<has_add_operator<T>::value, T>::type add(const T& a,
                                                                   const T& b)
  {
    return a + b;
  }
}  // namespace template_sfinae_example

namespace template_sfinae_example_test
{
  void test()
  {
    int x = 1, y = 2;
    auto result = template_sfinae_example::add(x, y);
    // 输出结果为 3
    std::cout << "Result of add(x, y): " << result << std::endl;

    struct X
    {
    };

    // 以下代码会导致编译错误，因为 X 类型不支持 operator+
    // X x1, x2;
    // auto x_result = template_sfinae_example::add(x1, x2);
  }
}  // namespace template_sfinae_example_test

/**
 * 疑问：
 * 1、这样有什么好处吗？使用了 SFINAE
 * 看起来还变复杂了。我就算不用这种写法，如果对象不支持
 * operator+，编译器也会报错啊。
 *
 * 2、虽然说 SFINAE
 * 可以影响重载决议，我知道这个很有用，但是上述的函数根本没有别的重载，这样写还有必要吗？
 *
 * 这两个问题其实是一个问题，本质上就是还不够懂 SFINAE 或者说模板：
 * 1、如果就是简单写一个 add 函数模板不使用
 * SFINAE，那么编译器在编译的时候，会尝试模板实例化，生成函数定义，发现你这类型根本没有 operator+，于是实例化模板错误。
 *
 * 2、如果按照我们上面的写法使用 SFINAE，根据 “代换失败不是错误”
 * 的规则，从重载集中丢弃这个特化 add，然而又没有其他的 add
 * 重载，所以这里的错误是未找到匹配的函数。

 * 这里的重点是什么？是模板实例化，能不要实例化就不要实例化，我们当前的示例只是因为 add 函数模板非常的简单，即使实例化错误，
 * 编译器依然可以很轻松的报错告诉你，是因为没有
 * operator+。但是很多模板是非常复杂的，编译器实例化模板经常会产生一些完全不可读的报错；如果我们使用
 * SFINAE，编译器就是直接告诉我：“未找到匹配的重载函数”，我们自然知道就是传入的参数没有满足要求。而且实例化模板也是有开销的，很多时候甚至很大。
 *
 * 总而言之：
 * 即使不为了处理重载，使用 SFINAE
 * 也是有好处的，可以避免不必要的模板实例化，减少编译时间，并且提供更清晰的错误信息。
 */