/**
 * @file strategy.cpp
 * @author cao zhang
 * @brief 策略模式的实现
 * 
 * 策略模式和状态模式的类 UML 图类似（结构类似），其抽象接口也大致相似，但是其区别在业务意图和运行逻辑
 * 
 * 1. 策略模式：同一个行为，多种算法实现
 * 例子：支付方式
 *  抽象策略：支付 Pay
 *  具体策略：微信支付、支付宝、银行卡
 *  客户端主动选用哪种支付
 *  微信、支付宝之间没有任何流转关系，你选哪个就用哪个，不会自动变
 * 👉 特点：我想用哪个就切哪个，算法平级、互不跳转
 * 
 * 2. 状态模式：同一个对象，不同状态行为不同，且会自动流转
 *  例子：订单状态
 *  抽象状态：订单状态 OrderState
 *  具体状态：待付款、已付款、已发货、已完成
 *  订单触发一个动作（付款 / 发货），自动从当前状态流转到下一个状态
 *  待付款不能直接跳到已完成，有固定状态机规则
 * 👉 特点：状态有先后顺序，由内部规则自动流转，外界只发指令不控制跳转
 * 
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
class StrategyAdd : public Strategy
{
 public:
  int execute(int a, int b) const override { return a + b; }
};

// 具体策略类：实现减法算法
class StrategySubtract : public Strategy
{
 public:
  int execute(int a, int b) const override { return a - b; }
};

// 具体策略类：实现乘法算法
class StrategyMultiply : public Strategy
{
 public:
  int execute(int a, int b) const override { return a * b; }
};

// 上下文类：维护策略对象，提供统一的调用接口
class Context
{
 public:
  explicit Context(std::unique_ptr<Strategy> strat = nullptr)
      : strategy_(std::move(strat))
  {
  }

  // 运行时切换策略的 setter 方法
  void setStrategy(std::unique_ptr<Strategy> strat)
  {
    strategy_ = std::move(strat);
  }

  int executeStrategy(int a, int b) const
  {
    if (!strategy_)
    {
      std::cerr << "Error: No strategy set!" << std::endl;
      return 0;  // 错误返回值
    }
    return strategy_->execute(a, b);
  }

 private:
  std::unique_ptr<Strategy> strategy_;
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
    context.setStrategy(std::make_unique<StrategyAdd>());
  }
  else if (action == "subtraction")
  {
    context.setStrategy(std::make_unique<StrategySubtract>());
  }
  else if (action == "multiplication")
  {
    context.setStrategy(std::make_unique<StrategyMultiply>());
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