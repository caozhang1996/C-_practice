/**
 * @file state.cpp
 * @author cao zhang
 * @brief 状态模式的实现
 * @version 0.1
 * @date 2025-02-08
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 状态接口
class State
{
 public:
  virtual ~State() = default;
  virtual std::string handle() = 0;  // 处理状态的方法
};

// 具体状态类
class OnState : public State
{
 public:
  std::string handle() override { return "Light is ON"; }
};

class OffState : public State
{
 public:
  std::string handle() override { return "Light is OFF"; }
};

class BlinkState : public State
{
 public:
  std::string handle() override { return "Light is Blinking"; }
};

// 上下文类
class Light
{
 public:
  Light() : state_(std::make_shared<OffState>()) {}  // 初始状态为关闭
  ~Light() = default;

  void setState(std::shared_ptr<State> new_state)
  {
    state_ = new_state;  // 设置新的状态
  }

  std::string performOperation()
  {
    return state_->handle();  // 执行当前状态的操作
  }

 private:
  std::shared_ptr<State> state_;  // 当前状态
};

int main()
{
  // 读取要输入的命令数量
  int n;
  std::cin >> n;
  std::cin.ignore();  // 消耗掉整数后的换行符

  // 创建一个Light对象
  Light light;

  // 处理用户输入的每个命令
  for (int i = 0; i < n; i++)
  {
    // 读取命令并去掉首尾空白字符
    std::string command;
    std::getline(std::cin, command);

    // 根据命令执行相应的操作
    if ("ON" == command)
    {
      light.setState(std::make_shared<OnState>());
    }
    else if ("OFF" == command)
    {
      light.setState(std::make_shared<OffState>());
    }
    else if ("BLINK" == command)
    {
      light.setState(std::make_shared<BlinkState>());
    }
    else
    {
      // 处理无效命令
      std::cout << "Invalid command: " << command << std::endl;
    }

    // 在每个命令后显示灯的当前状态
    std::cout << light.performOperation() << std::endl;
  }

  return 0;
}
