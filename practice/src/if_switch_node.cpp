#include <iostream>

// if/switch 初始化语句：允许在 if 或 switch 条件中直接声明变量，限制变量作用域

int getInt()
{
  return 1;
}

std::string getString()
{
  return "hello world!";
}

int main(int argc, char** argv)
{
  if (std::string str = getString(); !str.empty())
  {
    std::cout << str << std::endl;
  }

  // switch 中初始化变量
  switch (int i = getInt(); i)
  {
    case 1:
      std::cout << i << std::endl;
      break;
    default:
      std::cout << "unknown case";
      break;
  }

  return 0;
}