/**
 * @file my_unique_ptr.cpp
 * @author cao zhang
 * @brief 手写简易的 unique_ptr
 * 
 * @version 0.1
 * @date 2026-05-11
 */


#include <iostream>
#include <utility>   // std::move

template <typename T>
class unique_ptr
{
private:
    // 原始裸指针
    T* ptr = nullptr;

public:
    // 1. 默认构造
    unique_ptr() : ptr(nullptr) {}

    // 2. 原生指针构造
    explicit unique_ptr(T* p) : ptr(p) {}

    // 3. 析构函数：自动释放资源
    ~unique_ptr()
    {
        delete ptr;
        ptr = nullptr;
    }

    // ===================== 禁止拷贝 =====================
    // 禁用拷贝构造
    unique_ptr(const unique_ptr<T>&) = delete;

    // 禁用拷贝赋值
    unique_ptr<T>& operator=(const unique_ptr<T>&) = delete;

    // ===================== 支持移动 =====================
    // 移动构造
    unique_ptr(unique_ptr<T>&& other) noexcept
    {
        // 接管对方资源
        ptr = other.ptr;
        // 对方置空，放弃所有权
        other.ptr = nullptr;
    }

    // 移动赋值
    unique_ptr<T>& operator=(unique_ptr<T>&& other) noexcept
    {
        // 自移动防护
        if (this == &other)
            return *this;

        // 先释放当前持有资源
        delete ptr;

        // 接管新资源
        ptr = other.ptr;
        other.ptr = nullptr;

        return *this;
    }

    // ===================== 核心接口 =====================
    // 解引用
    T& operator*() const
    {
        return *ptr;
    }

    // 箭头访问成员
    T* operator->() const
    {
        return ptr;
    }

    // 获取裸指针
    T* get() const
    {
        return ptr;
    }

    // 释放管理权，不析构对象，返回裸指针
    T* release()
    {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // 重置：释放旧资源，接管新指针
    void reset(T* p = nullptr)
    {
        delete ptr;
        ptr = p;
    }

    // 判断是否为空
    explicit operator bool() const
    {
        return ptr != nullptr;
    }
};

// 测试用自定义类，看析构是否正常调用
struct TestObj
{
    TestObj()  
    { 
        std::cout << "TestObj 构造" << std::endl; 
    }
    
    ~TestObj() 
    { 
        std::cout << "TestObj 析构" << std::endl; 
    }
    
    void hello() 
    { 
        std::cout << "hello from TestObj" << std::endl; 
    }
};

int main(int argc, char** argv)
{
    // 1. 基础使用
    unique_ptr<TestObj> p1(new TestObj);
    p1->hello();

    // 2. 禁止拷贝（下面两行编译直接报错）
    // unique_ptr<TestObj> p2 = p1;
    // unique_ptr<TestObj> p3;
    // p3 = p1;

    // 3. 移动构造
    unique_ptr<TestObj> p2 = std::move(p1);
    if (!p1)
    {
        std::cout << "p1 已空，失去所有权" << std::endl;
    }
    p2->hello();

    // 4. 移动赋值
    unique_ptr<TestObj> p3;
    p3 = std::move(p2);
    if (!p2)
    {
        std::cout << "p2 已空，失去所有权" << std::endl;
    }

    // 5. release 放弃管理权
    TestObj* raw = p3.release();
    if (!p3)
    {
        std::cout << "p3 release 后为空" << std::endl;
    }
    // 手动释放
    delete raw;

    // 6. reset 重置
    unique_ptr<int> pi(new int(666));
    std::cout << *pi << std::endl;
    pi.reset(new int(888));
    std::cout << *pi << std::endl;

    return 0;
}