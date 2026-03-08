/**
 * @file template_dependent_names_1.h
 * @author cao zhang
 * @brief 待决名 消除歧义符
 * @version 0.1
 * @date 2025-01-18
 */

#pragma once

#include <vector>

// 待决定名（Dependent Names) 消除歧义符

/**
 * 在模板（包括别名模板）的声明或定义中，不是当前实例化的成员且取决于某个模板行参的名字不会被认为是类型，除非使用关键词
 * typename 或它已经被设立为类型名（例如用 typedef 声明）
 */

namespace template_dependent_names_1
{
  int p = 1;

  template <typename T>
  void foo(const std::vector<T>& vec)
  {
    // std::vector<T>::const_iterator 是待决名
    typename std::vector<T>::const_iterator it1 = vec.begin();

    // iter_t 是一个类型别名，已经被设立为类型名，所以不需要 typename 关键词
    typedef typename std::vector<T>::const_iterator iter_t;
    iter_t it2 = vec.end();
  }

};  // namespace template_dependent_names_1

namespace template_dependent_names_1_test
{
  void test()
  {
    std::vector<int> vec = {1, 2, 3};
    template_dependent_names_1::foo(vec);
  }
};  // namespace template_dependent_names_1_test