/**
 * @file enable_shared_from_this.cpp
 * @author cao zhang
 * @brief enable_shared_from_this 的简易使用
 * 
 * std::shared_from_this 常用于以下场景：
 *  ●回调函数：在异步操作中，需要将当前对象传递给回调函数。
 *  ●链式调用：在链式调用中，需要返回当前对象的 std::shared_ptr。
 *  ●观察者模式：在观察者模式中，需要将当前对象注册为观察者。
 * 
 * @version 0.1
 * @date 2026-05-11
 */

#include <iostream>
#include <memory>

class MyClass : public std::enable_shared_from_this<MyClass> 
{
public:
    void do_something() 
    {
        // 获取当前对象的 shared_ptr
        std::shared_ptr<MyClass> self = shared_from_this();
        std::cout << "Use count: " << self.use_count() << std::endl;
    }
};

int main() 
{
    std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
    obj->do_something();  // 输出: Use count: 2
}
