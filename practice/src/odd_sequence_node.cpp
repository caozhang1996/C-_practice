#include <array>
#include <iostream>
#include <utility>

// 示例模板类，使用奇数作为模板参数
template <int N>
struct Example
{
  Example()
  {
    std::cout << "Example instance created with N = " << N << std::endl;
  }

  static constexpr int value = N;
};

// 方法1：通过偶数索引生成奇数
template <size_t... Indices>
void instantiateOddExamples(std::index_sequence<Indices...>)
{
  // 使用公式 2*i+1 生成奇数
  auto dummy = {(Example<2 * static_cast<int>(Indices) + 1>{}, 0)...};
}

template <size_t N>
void createOddExamples()
{
  // 生成0到N-1的索引，然后转换为奇数
  instantiateOddExamples(std::make_index_sequence<N>{});
}

// 方法2：使用奇数序列直接实例化
template <size_t... Odds>
void instantiateOddExamplesDirect(std::index_sequence<Odds...>)
{
  [[maybe_unused]] auto dummy = {(Example<static_cast<int>(Odds)>{}, 0)...};
}

int main()
{
  // 方法1：生成前5个奇数
  std::cout << "Generating first 5 odd numbers:" << std::endl;
  createOddExamples<5>();

  // 方法2：直接使用奇数序列
  std::cout << "\nGenerating odd numbers from 1 to 9:" << std::endl;
  instantiateOddExamplesDirect(std::index_sequence<1, 3, 5, 7, 9>{});

  return 0;
}
