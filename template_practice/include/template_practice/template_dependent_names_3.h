/**
 * @file template_dependent_names_3.h
 * @author cao zhang
 * @brief 待决与非待决的查找规则
 * @version 0.1
 * @date 2025-01-18
 */

#pragma once

#include <iostream>

void f()
{
  std::cout << "goal f function" << std::endl;
}

template <typename T>
struct X
{
  void f() { std::cout << "X<T>::f function" << std::endl; }
};

template <typename T>
struct Y : public X<T>
{
  // f()
  // 是非待决名，检查该模板的定义时将进行无限定的名字查找（无法查找父类的定义），按照正常的查看顺序，先类内（查找不到），然后全局（找到）
  void t1() { f(); }

  // this->f()
  // 是待决名，所以它的查找会推迟到得知它模板实参之时（届时可以确定父类是否有 f
  // 函数）
  void t2() { this->f(); }
};

void test_dependent_names_3()
{
  Y<int> y;
  y.t1();
  y.t2();
}