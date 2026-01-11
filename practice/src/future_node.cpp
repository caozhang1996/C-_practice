#include <future>
#include <iostream>
#include <thread>

int task()
{
  // todo...
  return 10;
}

void threadFunc(std::future<int>& fut)
{
  // todo...

  try
  {
    int result = fut.get();
    std::cout << result << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  // todo...
}

int main(int argc, char** argv)
{
  auto fut = std::async(task);

  std::thread th1 = std::thread(threadFunc, std::ref(fut));
  std::thread th2 = std::thread(threadFunc, std::ref(fut));

  th1.join();
  th2.join();

  return 0;
}
