
#include <functional>
#include <iostream>
#include <thread>

struct X
{
  void foo(int a) { std::cout << "a: " << a << std::endl; }

  void operator()(int a) { std::cout << "operator(): " << a << std::endl; }
};

void add(int a, int b)
{
  std::cout << "a + b = " << a + b << std::endl;
}

void testInvoke()
{
  // 成员函数: 传递值和地址都是可以的
  X x;
  std::invoke(&X::foo, x, 10);
  std::invoke(&X::foo, &x, 10);

  // 函数
  std::invoke(add, 100, 200);

  // 函数对象
  std::invoke(x, 111);

  // Lambda 表达式
  auto lambda = [](int a, int b) {
    return a + b;
  };
  int sum = std::invoke(lambda, 3, 5);  // 等价于 lambda(3, 5)
  std::cout << "sum: " << sum << std::endl;
}

void sizeofThread()
{
  std::cout << "sizof(std::thread): " << sizeof(std::thread) << std::endl;
  std::cout << "sizof(pthread_t): " << sizeof(pthread_t) << std::endl;
}

int main(int argc, char** argv)
{
  testInvoke();
  return 0;
}