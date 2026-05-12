/**
 * @file abstract_factory.cpp
 * @author cao zhang
 * @brief 工厂方法模式
 * 
 * 涉及到创建“多类”对象了，在工厂方法模式中，每个具体工厂只负责创建单一的产品。
 * 但是如果有多类产品呢，比如说“手机”，一个品牌的手机有高端机、中低端机之分，
 * 这些具体的产品都需要建立一个单独的工厂类，但是它们都是相互关联的，都共同属于同一个品牌，
 * 这就可以使用到【抽象工厂模式】。
 * 
 * 抽象工厂模式可以确保一系列相关的产品被一起创建，这些产品能够相互配合使用，再举个例子，
 * 有一些家具，比如沙发、茶几、椅子，都具有古典风格的和现代风格的，抽象工厂模式可以将生产
 * 现代风格的家具放在一个工厂类中，将生产古典风格的家具放在另一个工厂类中，这样每个工厂类就可以生产一系列的家具。
 * @version 0.1
 * @date 2025-05-12
 */

#include <iostream>
#include <string>
 
// 抽象椅子接口
class Chair 
{
public:
    virtual void showInfo() = 0;
};
 
// 具体现代风格椅子
class ModernChair : public Chair 
{
public:
    void showInfo() override 
    {
        std::cout << "modern chair" << std::endl;
    }
};
 
// 具体古典风格椅子
class ClassicalChair : public Chair 
{
public:
    void showInfo() override 
    {
        std::cout << "classical chair" << std::endl;
    }
};
 
// 抽象沙发接口
class Sofa 
{
public:
    virtual void displayInfo() = 0;
};
 
// 具体现代风格沙发
class ModernSofa : public Sofa 
{
public:
    void displayInfo() override 
    {
        std::cout << "modern sofa" << std::endl;
    }
};
 
// 具体古典风格沙发
class ClassicalSofa : public Sofa 
{
public:
    void displayInfo() override 
    {
        std::cout << "classical sofa" << std::endl;
    }
};
 
// 抽象家居工厂接口
class FurnitureFactory 
{
public:
    virtual Chair* createChair() = 0;
    virtual Sofa* createSofa() = 0;
};
 
// 具体现代风格家居工厂
class ModernFurnitureFactory : public FurnitureFactory 
{
public:
    Chair* createChair() override 
    {
        return new ModernChair();
    }
 
    Sofa* createSofa() override 
    {
        return new ModernSofa();
    }
};
 
// 具体古典风格家居工厂
class ClassicalFurnitureFactory : public FurnitureFactory 
{
public:
    Chair* createChair() override 
    {
        return new ClassicalChair();
    }
 
    Sofa* createSofa() override 
    {
        return new ClassicalSofa();
    }
};
 
int main(int argc, char** argv)
{
    // 读取订单数量
    int N;
    std::cin >> N;
 
    // 处理每个订单
    for (int i = 0; i < N; i++) 
    {
        // 读取家具类型
        std::string furnitureType;
        std::cin >> furnitureType;
 
        // 创建相应风格的家居装饰品工厂
        FurnitureFactory* factory = nullptr;
        if (furnitureType == "modern") 
        {
            factory = new ModernFurnitureFactory();
        } 
        else if (furnitureType == "classical") 
        {
            factory = new ClassicalFurnitureFactory();
        }
 
        // 根据工厂生产椅子和沙发
        Chair* chair = factory->createChair();
        Sofa* sofa = factory->createSofa();
 
        // 输出家具信息
        chair->showInfo();
        sofa->displayInfo();
 
        // 释放动态分配的对象
        delete chair;
        delete sofa;
        delete factory;
    }
 
    return 0;
}

