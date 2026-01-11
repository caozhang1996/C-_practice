#include <cmath>
#include <iostream>
#include <vector>

class Shape;

// 访问者接口
class Visitor
{
 public:
  virtual void visit(class Circle& circle) = 0;
  virtual void visit(class Rectangle& rectangle) = 0;
};

// 元素接口
class Shape
{
 public:
  virtual ~Shape() {}  // 添加虚析构函数
  virtual void accept(Visitor& visitor) = 0;
};

// 具体元素类
class Circle : public Shape
{
 public:
  Circle(int radius) : radius_(radius) {}

  int getRadius() const { return radius_; }

  void accept(Visitor& visitor) override { visitor.visit(*this); }

 private:
  int radius_;
};

// 具体元素类
class Rectangle : public Shape
{
 public:
  Rectangle(int width, int height) : width_(width), height_(height) {}

  int getWidth() const { return width_; }

  int getHeight() const { return height_; }

  void accept(Visitor& visitor) override { visitor.visit(*this); }

 private:
  int width_;
  int height_;
};

// 具体访问者类 1：面积
class AreaCalculator : public Visitor
{
 public:
  void visit(Circle& circle) override
  {
    double area = 3.14 * std::pow(circle.getRadius(), 2);
    std::cout << "area: " << area << std::endl;
  }

  void visit(Rectangle& rectangle) override
  {
    int area = rectangle.getWidth() * rectangle.getHeight();
    std::cout << "area: " << area << std::endl;
  }
};

// 具体访问者类 2：周长
class PerimeterCalculator : public Visitor
{
 public:
  void visit(Circle& circle) override
  {
    double perimeter = 2 * 3.14 * circle.getRadius();
    std::cout << "perimeter: " << perimeter << std::endl;
  }

  void visit(Rectangle& rectangle) override
  {
    int perimeter = 2 * (rectangle.getWidth() + rectangle.getHeight());
    std::cout << "perimeter: " << perimeter << std::endl;
  }
};

// 对象结构类
class Drawing
{
 public:
  Drawing(const std::vector<Shape*>& shapes) : shapes_(shapes) {}

  void accept(Visitor& visitor)
  {
    for (Shape* shape : shapes_)
    {
      shape->accept(visitor);
    }
  }

 private:
  std::vector<Shape*> shapes_;
};

int main()
{
  std::vector<Shape*> shapes;

  shapes.push_back(new Circle(5));
  shapes.push_back(new Circle(2));

  shapes.push_back(new Rectangle(3, 3));
  shapes.push_back(new Rectangle(2, 5));

  Drawing drawing(shapes);
  AreaCalculator areaCalculator;
  PerimeterCalculator perimeterCalculator;

  drawing.accept(areaCalculator);
  drawing.accept(perimeterCalculator);

  // 释放动态分配的内存
  for (Shape* shape : shapes)
  {
    delete shape;
  }
  return 0;
}
