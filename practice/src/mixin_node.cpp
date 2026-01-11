#include <iostream>
#include <string>

// 1. 定义 Mixin 类（以“可日志”和“可重置”为例）
template <typename T>
class LoggableMixin
{
 public:
  void log(const std::string& msg)
  {
    static_cast<T*>(this)->doLog(msg);  // 依赖目标类的 doLog()
  }
};

template <typename T>
class ResettableMixin
{
 public:
  void reset()
  {
    static_cast<T*>(this)->doReset();  // 依赖目标类的 doReset()
  }
};

// 组合器
template <template <typename> class... Mixins>
struct Compose
{
  template <typename Derived>
  struct Type : Mixins<Derived>...
  {
  };
};

// 3. 目标类
class Task : public Compose<LoggableMixin, ResettableMixin>::Type<Task>
{
 public:
  Task() : count(0) {}

  void run()
  {
    reset();
    log("Has reset the count!");
  }

  // 实现 LoggableMixin 依赖的 doLog()
  void doLog(const std::string& msg) { std::cout << msg << std::endl; }

  // 实现 ResettableMixin 依赖的 doReset()
  void doReset()
  {
    std::cout << "do reset" << std::endl;
    count = 0;
  }

 private:
  int count;
};

// 测试
int main()
{
  Task task;
  task.run();
  return 0;
}