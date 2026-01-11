// 字符串字面量运算符

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

// 处理整数小时（如 1h -> 3600 秒）
unsigned long long operator""_h(unsigned long long hours)
{
  return hours * 3600;
}

// 处理浮点小时（如 2.5h -> 9000 秒）
unsigned long long operator""_h(long double hours)
{
  return static_cast<unsigned long long>(hours * 3600);
}

int main()
{
  std::cout << 1_h << " 秒" << std::endl;    // 输出：3600 秒
  std::cout << 2.5_h << " 秒" << std::endl;  // 输出：9000 秒

  std::cout << (10h).count() << " 小时" << std::endl;
  std::cout << (100ns).count() << " 纳秒" << std::endl;

  // 时间段转换截断
  std::chrono::milliseconds ms(3999);
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ms);
  std::cout << seconds.count() << " 秒" << std::endl;

  return 0;
}