/**
 * @file virtual_table_ptr.cpp
 * @author cao zhang
 * @brief 虚函数表指针测试
 * 
 * bar函数能够正常调用，但是foo函数不能，原因是bar函数中没有涉及到对空指针的解引用，
 * 如果涉及到对空指针的解引用，仍然是不能正常调用的。foo函数无法调用是因为没有创建ClassA的对象，也就没有虚函数表，
 * 通过ptr解引用找到虚函数表时因为ptr是nullptr导致段错误。
 * @version 0.1
 * @date 2026-05-10
 */

#include <iostream>

class ClassA
{
public:
    virtual void foo()
    {
        std::cout << "foo" << std::endl;
    }
    
    void bar()
    {
        std::cout << "bar" << std::endl;
    }
};

int main(int argc, char** argv)
{
    ClassA* ptr = nullptr;
    ptr->bar();
    ptr->foo();
    return 0;
}
