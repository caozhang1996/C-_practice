/**
 * CRTP: CRTP（Curiously Recurring Template
 * Pattern，奇异递归模板模式）是一种基于模板的设计模式
 */

#include <iostream>

/**
 * 静态多态（替代虚函数）: 实现 “不同类型对象执行相似逻辑”，但无需虚函数
 */

// 模板基类：定义通用接口
template <typename Shape>
class ShapeBase
{
 public:
  double getArea() const
  {
    // 调用派生类的面积计算逻辑
    return static_cast<const Shape*>(this)->calculateArea();
  }
};

// 派生类 1：圆形
class Circle : public ShapeBase<Circle>
{
 public:
  Circle(double r) : radius(r) {}
  // 实现基类依赖的接口
  double calculateArea() const { return 3.14159 * radius * radius; }

 private:
  double radius;
};

// 派生类 2：矩形
class Rectangle : public ShapeBase<Rectangle>
{
 public:
  Rectangle(double w, double h) : width(w), height(h) {}
  double calculateArea() const { return width * height; }

 private:
  double width, height;
};

// 通用函数：接收任意 ShapeBase 派生类
template <typename Shape>
void printArea(const ShapeBase<Shape>& shape)
{
  std::cout << "Area: " << shape.getArea() << std::endl;
}

int main(int argc, char** argv)
{
  Circle c(5);
  Rectangle r(3, 4);

  std::cout << "Area: " << c.getArea() << std::endl;
  std::cout << "Area: " << r.getArea() << std::endl;

  return 0;
}