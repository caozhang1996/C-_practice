/**
 * @file custom_deleter.cpp
 * @author cao zhang
 * @brief 虚函数表指针测试
 * 
 * ● 如果使用函数指针作为删除器，删除器的函数签名必须符合 void(*)(T*) 的格式，其中 T 
 *   是指向的对象类型。也就是说，删除器函数必须接受一个指向对象类型 T 的指针作为参数，并且返回类型为 void
 * 
 * ● std::unique_ptr 的删除器类型是通过模板参数传递的，需要在创建对象时显式指定；而 
 *   std::shared_ptr 的删除器类型是作为构造函数参数传递的，不需要显式指定类型。
 * @version 0.1
 * @date 2026-05-11
 */

#include <iostream>
#include <string>
#include <memory>

 /**
    删除器是函数指针
*/
inline void customDeleter(int* _ptr) 
{
    std::cout << "[fucntion pointer deleter] Custom deleter is called. Deleting pointer...\n";
    delete _ptr;
}

inline void testCustomDeleter()
{
    std::unique_ptr<int, void(*)(int*)> ptr(new int(42), customDeleter);

    // 使用智能指针
    std::cout << "Value of pointer: " << *ptr << std::endl;
}

/**
    删除器是 lambda 函数
*/
inline void testLabdmaDeleter()
{
    auto func = [](int* _ptr) {
        std::cout << "[lambda deleter] Custom deleter is called. Deleting pointer...\n";
        delete _ptr;
    };

    std::unique_ptr<int, decltype(func)> ptr(new int(50), func);

    // 使用智能指针
    std::cout << "Value of pointer: " << *ptr << std::endl;
}

/**
    删除器是仿函数
*/
struct Deleter
{
    void operator()(int* _ptr) const 
    {
        std::cout << "[functor deleter] Custom deleter is called. Deleting pointer...\n";
        delete _ptr;
    }
};

inline void testFunctor()
{
    std::unique_ptr<int, Deleter> ptr(new int(65), Deleter{});

    // 使用智能指针
    std::cout << "Value of pointer: " << *ptr << std::endl;
}


int main(int argc, char** argv)
{
    testCustomDeleter();
    testLabdmaDeleter();
    testFunctor();

    return 0;
}