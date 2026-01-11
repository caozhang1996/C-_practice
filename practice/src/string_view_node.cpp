#include <iostream>
#include <string>
#include <string_view>

// 用 std::string 作为参数：会触发深拷贝（如果传入临时字符串）
void print_string(std::string s)
{
  std::cout << s << " (size: " << s.size() << ")\n";
}

// 用 std::string_view 作为参数：仅传递视图，无拷贝
void print_view(std::string_view sv)
{
  std::cout << sv << " (size: " << sv.size() << ")\n";
}

void test_1()
{
  std::string str = "hello";

  // std::string 会拷贝数据
  print_string(str);      // 传递 str 的拷贝
  print_string("world");  // 隐式构造临时 string，拷贝 "world"

  // std::string_view 仅指向数据
  print_view(str);      // 指向 str 的数据（无拷贝）
  print_view("world");  // 指向字面量 "world"（无拷贝）
  print_view(str.substr(
      1, 3));  // 指向 str 中 "ell" 的视图（避免 substr 产生的临时 string 拷贝）
}

std::string_view bad_view()
{
  std::string s = "temporary";
  return std::string_view(s);  // 错误：s 销毁后，视图悬空
}

void test_2()
{
  std::string_view sv = bad_view();

  std::cout << sv << " (size: " << sv.size() << ")\n";
}

int main()
{
  test_2();
  return 0;
}
