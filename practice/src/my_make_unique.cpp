/**
 * @file my_make_unique.cpp
 * @author cao zhang
 * @brief 虚函数表指针测试
 * 
 * C++ 11 版本没有 std::make_unique 函数来方便地构造一个 std::unique_ptr 对象，这里尝试
 * 写一个通用的方便的 std::make_unique 
 * 
 * @version 0.1
 * @date 2026-05-11
 */

#include <iostream>
#include <memory>
#include <string>

template <typename T, typename ...Args>
inline std::unique_ptr<T> make_unique(Args&& ..._args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(_args)...));
}

// 如果有一个类构造函数如下，则可这样使用make_unique函数
class Student
{
public:
    Student(const std::string& _name, uint8_t _age): name_(_name), age_(_age) { }

    void printInfo()
    {
        // uint8_t 类型不能正常打印，需要转换一下类型
        std::cout << "name: " << name_ << ", age: " << static_cast<uint32_t>(age_) << std::endl;
    }

private:
    std::string name_;
    uint8_t age_;
};

int main(int argc, char** argv)
{
    std::unique_ptr<Student> stuPtr = make_unique<Student>("Tom", 20);
    stuPtr->printInfo();

    return 0;
}
