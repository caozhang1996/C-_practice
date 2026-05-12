/**
 * @file factory_method.cpp
 * @author cao zhang
 * @brief 工厂方法模式
 * 
 * 工厂方法模式引入了抽象工厂和具体工厂的概念，每个具体工厂只负责创建一个具体产品，添加新的产品只需要添加新的工厂类
 * @version 0.1
 * @date 2025-05-12
 */

#include <iostream>
#include <vector>
 
// 抽象积木接口
class Block 
{
public:
    virtual void produce() = 0;
};
 
// 具体圆形积木实现
class CircleBlock : public Block 
{
public:
    void produce() override 
    {
        std::cout << "Circle Block" << std::endl;
    }
};
 
// 具体方形积木实现
class SquareBlock : public Block 
{
public:
    void produce() override 
    {
        std::cout << "Square Block" << std::endl;
    }
};
 
// 抽象积木工厂接口
class BlockFactory 
{
public:
    virtual Block* createBlock() = 0;
};
 
// 具体圆形积木工厂实现
class CircleBlockFactory : public BlockFactory 
{
public:
    Block* createBlock() override 
    {
        return new CircleBlock();
    }
};
 
// 具体方形积木工厂实现
class SquareBlockFactory : public BlockFactory 
{
public:
    Block* createBlock() override 
    {
        return new SquareBlock();
    }
};
 
// 积木工厂系统
class BlockFactorySystem 
{
private:
    std::vector<Block*> blocks;
 
public:
    void produceBlocks(BlockFactory* factory, int quantity) 
    {
        for (int i = 0; i < quantity; i++) {
            Block* block = factory->createBlock();
            blocks.push_back(block);
            block->produce();
        }
    }
 
    const std::vector<Block*>& getBlocks() const 
    {
        return blocks;
    }
 
    ~BlockFactorySystem() 
    {
        // 释放所有动态分配的积木对象
        for (Block* block : blocks) 
        {
            delete block;
        }
    }
};
 
int main() 
{
    // 创建积木工厂系统
    BlockFactorySystem factorySystem;
 
    // 读取生产次数
    int productionCount;
    std::cin >> productionCount;
 
    // 读取每次生产的积木类型和数量
    for (int i = 0; i < productionCount; i++) 
    {
        std::string blockType;
        int quantity;
        std::cin >> blockType >> quantity;
 
        if ("Circle" == blockType)
        {
            factorySystem.produceBlocks(new CircleBlockFactory(), quantity);
        } 
        else if ("Square" == blockType) 
        {
            factorySystem.produceBlocks(new SquareBlockFactory(), quantity);
        }
    }
 
    return 0;
}