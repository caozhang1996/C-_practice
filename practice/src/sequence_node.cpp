#include <iostream>
#include <utility>

// 1. 定义一个基础模板类，用于表示索引序列
template <int... Indices>
struct IndexSequence
{
};

// 2. 定义模板递归的终止条件：生成空序列
template <int N, int... Indices>
struct MakeIndexSequenceHelper
    : MakeIndexSequenceHelper<N - 1, N - 1, Indices...>
{
};

template <int... Indices>
struct MakeIndexSequenceHelper<0, Indices...>
{
  using type = IndexSequence<Indices...>;
};

// 3. 定义一个便捷的别名模板，简化使用
template <int N>
using MakeIndexSequence = typename MakeIndexSequenceHelper<N>::type;

// 4. 示例模板类，我们将使用索引序列来实例化它
template <int N>
struct Example
{
  Example()
  {
    std::cout << "Example instance created with N = " << N << std::endl;
  }
};

// 5. 辅助函数：使用索引序列实例化多个Example对象
template <int... Indices>
void instantiateExamples(IndexSequence<Indices...>)
{
  // 使用初始化列表展开参数包，创建多个Example实例
  auto dummy = {(Example<Indices>{}, 0)...};
}

// 6. 包装函数：生成索引序列并调用实例化函数
template <int N>
void createExamples()
{
  instantiateExamples(MakeIndexSequence<N>{});
}

int main()
{
  // 示例：创建N=5的索引序列，并实例化Example<0>到Example<4>
  std::cout << "Creating examples with indices from 0 to 4:" << std::endl;
  createExamples<5>();

  return 0;
}
