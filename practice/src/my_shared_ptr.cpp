/**
 * @file my_shared_ptr.cpp
 * @author cao zhang
 * @brief 虚函数表指针测试
 * 
 * @version 0.1
 * @date 2026-05-11
 */

#include <iostream>


template <typename T>
class shared_ptr
{
public:
    // 默认构造函数：初始化为空指针
    shared_ptr() : ptr(nullptr), ref_count(nullptr) {}

    // 带原始指针的构造函数（显式，防止隐式转换）
    explicit shared_ptr(T* p) : ptr(p)
    {
        ref_count = new int(1);
    }

    // 拷贝构造函数
    shared_ptr(const shared_ptr<T>& other) : ptr(other.ptr), ref_count(other.ref_count)
    {
        (*ref_count)++;
    }

    // 析构函数
    ~shared_ptr()
    {
        release();
    }

    // 拷贝赋值运算符
    shared_ptr<T>& operator=(const shared_ptr<T>& other)
    {
        if (this == &other) return *this;

        if (nullptr == other.ptr)
        {
            release();
            return *this;
        }

        if (this != &other)
        {
            // 先释放当前资源
            release();

            // 接管新资源
            ptr = other.ptr;
            ref_count = other.ref_count;
            (*ref_count)++;
        }

        return *this;
    }

    // 解引用运算符
    T& operator*() const 
    { 
        if (!ptr) 
            throw std::runtime_error("空指针解引用");
        
        return *ptr; 
    }

    // 箭头运算符
    T* operator->() const 
    {
        if (!ptr) 
            throw std::runtime_error("空指针访问"); 
        
        return ptr; 
    }

    // 获取当前引用计数
    int count() const { return ref_count? *ref_count : 0; }

private:
    // 释放资源的辅助函数
    void release()
    {
        // 先判断空！！！
        if (nullptr == ref_count)
            return;
        
        (*ref_count)--;

        if (*ref_count == 0)
        {
            delete ptr;
            delete ref_count;

            // 好习惯：置空
            ptr = nullptr;
            ref_count = nullptr;
        }
    }

    T* ptr;         // 指向管理对象的原始指针
    int* ref_count; // 指向引用计数的指针
};

int main()
{
    std::cout << "=== 测试 1: 构造单个 shared_ptr ===" << std::endl;
    shared_ptr<int> p1(new int(100));
    std::cout << "p1 指向的值：" << *p1 << std::endl;
    std::cout << "p1 引用计数：" << p1.count() << std::endl << std::endl;

    std::cout << "=== 测试 2: 拷贝构造 ===" << std::endl;
    shared_ptr<int> p2 = p1;
    std::cout << "p1 计数：" << p1.count() << std::endl;
    std::cout << "p2 计数：" << p2.count() << std::endl;
    std::cout << "*p2 = " << *p2 << std::endl << std::endl;

    std::cout << "=== 测试 3: 赋值运算符 ===" << std::endl;
    shared_ptr<int> p3;
    p3 = p1;
    std::cout << "p1 计数：" << p1.count() << std::endl;
    std::cout << "p3 计数：" << p3.count() << std::endl;
    std::cout << "*p3 = " << *p3 << std::endl << std::endl;

    std::cout << "=== 测试 4: 修改值（所有指针共享同一块内存）===" << std::endl;
    *p1 = 999;
    std::cout << "*p1 = " << *p1 << std::endl;
    std::cout << "*p2 = " << *p2 << std::endl;
    std::cout << "*p3 = " << *p3 << std::endl << std::endl;

    std::cout << "=== 测试 5: 销毁一个指针（计数减 1）===" << std::endl;
    {
        shared_ptr<int> p4 = p1;
        std::cout << "p4 加入后计数：" << p1.count() << std::endl;
    } // p4 离开作用域自动销毁
    std::cout << "p4 销毁后计数：" << p1.count() << std::endl << std::endl;

    std::cout << "=== 测试 6: 空指针 ===" << std::endl;
    shared_ptr<int> p_empty;
    std::cout << "空指针计数：" << p_empty.count() << std::endl << std::endl;

    std::cout << "=== 测试 7: 最后全部销毁，内存自动释放 ===" << std::endl;
    return 0;
}