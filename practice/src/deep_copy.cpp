/**
 * @file deep_copy.cpp
 * @author cao zhang
 * @brief 深拷贝示例
 * 
 * 遵循 C++ 三大法则（Rule of Three）：只要类中有动态内存分配，就必须实现：
 * 1、拷贝构造函数
 * 2、赋值运算符重载
 * 3、析构函数
 * 
 * @version 0.1
 * @date 2026-05-10
 */

#include <iostream>

class Base 
{
public:
    Base() 
    {
        std::cout << "Base constructor called." << std::endl;
    }

    virtual ~Base() 
    {
        std::cout << "Base destructor called." << std::endl;
    }

    virtual void print() 
    {
        std::cout << "Printing from Base." << std::endl;
    }
};

class Derived : public Base 
{
private:
    int* data;    // 动态数组指针
    int size;     // 动态大小

public:
    // 1. 构造函数：可指定任意大小
    Derived(int arraySize = 5): size(arraySize)
    {
        std::cout << "Derived 构造，大小：" << size << std::endl;
        data = new int[size]();  // 动态分配
    }

    // 2. 拷贝构造函数：深拷贝（支持动态大小）
    Derived(const Derived& other) 
    {
        std::cout << "Derived 拷贝构造（深拷贝）" << std::endl;
        
        // 复制对方的大小
        size = other.size;
        
        // 【关键】开辟新内存，不共享指针
        data = new int[size];
        for (int i = 0; i < size; ++i) 
        {
            data[i] = other.data[i];
        }
    }

    // 3. 赋值运算符重载：深拷贝（支持动态大小）
    Derived& operator=(const Derived& other) 
    {
        std::cout << "Derived 赋值重载（深拷贝）" << std::endl;
        
        if (this == &other) 
        {
            return *this;  // 防止自赋值
        }

        // 先释放自己的旧内存
        delete[] data;

        // 复制新大小 + 分配新内存
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; ++i) 
        {
            data[i] = other.data[i];
        }

        return *this;
    }

    // 4. 析构函数
    ~Derived() 
    {
        std::cout << "Derived 析构" << std::endl;
        delete[] data;
    }

    void print() override 
    {
        std::cout << "Derived::print() 大小：" << size << std::endl;
    }

    // 赋值
    void setValue(int index, int value) 
    {
        if (index >= 0 && index < size)
            data[index] = value;
    }

    // 打印数组
    void showArray() const 
    {
        std::cout << "数组[" << size << "]: ";
        for (int i = 0; i < size; ++i) 
        {
            std::cout << data[i] << " ";
        }
        
        std::cout << std::endl;
    }

    // 获取当前数组大小
    int getSize() const 
    {
        return size;
    }
};

int main() 
{
    std::cout << "===== 创建 d1: 大小 10 =====" << std::endl;
    Derived d1(10);
    d1.setValue(0, 100);
    d1.setValue(1, 200);
    d1.showArray();

    std::cout << "\n===== 拷贝构造 d2 = d1 (自动继承大小) =====" << std::endl;
    Derived d2 = d1;
    d2.showArray();

    std::cout << "\n===== 修改 d2, d1 不受影响 =====" << std::endl;
    d2.setValue(0, 9999);
    d1.showArray();
    d2.showArray();

    std::cout << "\n===== 创建 d3: 大小 3 =====" << std::endl;
    Derived d3(3);
    d3.setValue(0, 111);
    d3.showArray();

    std::cout << "\n===== 赋值重载: d3 = d2 =====" << std::endl;
    d3 = d2;
    d3.showArray();

    std::cout << "\n===== 结束 =====" << std::endl;
    return 0;
}