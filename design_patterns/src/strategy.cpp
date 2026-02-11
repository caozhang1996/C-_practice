/**
 * @file strategy.cpp
 * @author cao zhang
 * @brief 策略模式的实现
 * @version 0.1
 * @date 2025-02-11
 */

#include <iostream>
#include <memory>  // 用于智能指针
#include <string>

// 策略接口（抽象类）：声明所有算法的通用接口
class Strategy
{
 public:
  virtual ~Strategy() = default;  // 虚析构函数，确保子类正确析构
  virtual int execute(int a, int b) const = 0;
};

// 具体策略类：实现加法算法
class ConcreteStrategyAdd : public Strategy
{
 public:
  int execute(int a, int b) const override { return a + b; }
};

// 具体策略类：实现减法算法
class ConcreteStrategySubtract : public Strategy
{
 public:
  int execute(int a, int b) const override { return a - b; }
};

// 具体策略类：实现乘法算法
class ConcreteStrategyMultiply : public Strategy
{
 public:
  int execute(int a, int b) const override { return a * b; }
};

// 上下文类：维护策略对象，提供统一的调用接口
class Context
{
 public:
  explicit Context(std::unique_ptr<Strategy> strat = nullptr)
      : strategy(std::move(strat))
  {
  }

  // 运行时切换策略的 setter 方法
  void setStrategy(std::unique_ptr<Strategy> strat)
  {
    strategy = std::move(strat);
  }

  int executeStrategy(int a, int b) const
  {
    if (!strategy)
    {
      std::cerr << "Error: No strategy set!" << std::endl;
      return 0;  // 错误返回值
    }
    return strategy->execute(a, b);
  }

 private:
  std::unique_ptr<Strategy> strategy;
};

// 客户端代码：交互逻辑 + 策略选择
int main()
{
  // 创建上下文对象
  Context context;

  // 读取用户输入的两个数字
  int a, b;
  std::cout << "Enter first number: ";
  std::cin >> a;
  std::cout << "Enter second number: ";
  std::cin >> b;

  // 读取用户选择的操作
  std::string action;
  std::cout << "Enter action (addition/subtraction/multiplication): ";
  std::cin >> action;

  // 根据用户输入选择并设置策略
  if (action == "addition")
  {
    context.setStrategy(std::make_unique<ConcreteStrategyAdd>());
  }
  else if (action == "subtraction")
  {
    context.setStrategy(std::make_unique<ConcreteStrategySubtract>());
  }
  else if (action == "multiplication")
  {
    context.setStrategy(std::make_unique<ConcreteStrategyMultiply>());
  }
  else
  {
    std::cerr << "Error: Invalid action!" << std::endl;
    return 1;  // 退出程序，返回错误码
  }

  // 执行策略并输出结果
  int result = context.executeStrategy(a, b);
  std::cout << "Result: " << result << std::endl;

  return 0;
}