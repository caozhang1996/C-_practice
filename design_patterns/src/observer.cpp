#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 前向声明
class Subject;

// 观察者接口
class Observer
{
 public:
  virtual ~Observer() = default;
  virtual void update(const std::string& message) = 0;
  virtual std::string name() const = 0;
};

// 主题类：维护观察者列表并发送通知
class Subject
{
 private:
  // 使用weak_ptr存储观察者，不影响其生命周期
  std::vector<std::weak_ptr<Observer>> observers;

 public:
  // 注册观察者
  void attach(std::shared_ptr<Observer> observer)
  {
    observers.push_back(observer);
    std::cout << "观察者 " << observer->name() << " 已注册\n";
  }

  // 发送通知给所有存活的观察者
  void notify(const std::string& message)
  {
    std::cout << "\n主题发送通知: " << message << "\n";
    std::cout << "开始通知存活的观察者...\n";

    // 复制为shared_ptr列表，过滤已销毁的观察者
    std::vector<std::shared_ptr<Observer>> alive_observers;
    for (auto& weak_obs : observers)
    {
      if (auto shared_obs = weak_obs.lock())
      {
        alive_observers.push_back(shared_obs);
      }
    }

    // 通知所有存活的观察者
    for (auto& obs : alive_observers)
    {
      obs->update(message);
    }
  }
};

// 具体观察者实现
class ConcreteObserver : public Observer
{
 private:
  std::string observer_name;

 public:
  ConcreteObserver(std::string name) : observer_name(std::move(name))
  {
    std::cout << "观察者 " << observer_name << " 被创建\n";
  }

  ~ConcreteObserver() override
  {
    std::cout << "观察者 " << observer_name << " 被销毁\n";
  }

  void update(const std::string& message) override
  {
    std::cout << "观察者 " << observer_name << " 收到消息: " << message << "\n";
  }

  std::string name() const override { return observer_name; }
};

int main()
{
  // 创建主题
  Subject subject;

  // 创建观察者并注册
  auto observer1 = std::make_shared<ConcreteObserver>("Observer 1");
  auto observer2 = std::make_shared<ConcreteObserver>("Observer 2");
  subject.attach(observer1);
  subject.attach(observer2);

  // 第一次通知：两个观察者都存活
  subject.notify("第一次通知 - 系统启动");

  // 销毁其中一个观察者
  std::cout << "\n手动销毁观察者 2...\n";
  observer2.reset();

  // 第二次通知：新增观察者3
  auto observer3 = std::make_shared<ConcreteObserver>("Observer 3");
  subject.attach(observer3);  // 新增观察者
  subject.notify("第二次通知 - 数据更新");

  return 0;
}
