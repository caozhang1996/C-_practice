#include <iostream>

#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 段错误处理函数
void handle_segv(int sig)
{
  void* callstack[128];
  int frames = backtrace(callstack, 128);
  char** symbols = backtrace_symbols(callstack, frames);

  std::cerr << "\n=============================================\n";
  std::cerr << "捕获到信号 " << sig << " (" << strsignal(sig) << ")\n";
  std::cerr << "调用栈（共 " << frames << " 帧）：\n";

  // 遍历解析后的符号并打印
  for (int i = 0; i < frames; ++i)
  {
    std::cerr << "  [" << i << "] " << symbols[i] << "\n";
  }

  std::cerr << "=============================================\n";

  free(symbols);  // 释放backtrace_symbols分配的内存

  // 退出程序
  exit(1);
}

// 用于演示调用栈的函数
void function_c()
{
  // 触发段错误：访问空指针
  int* null_ptr = nullptr;
  *null_ptr = 42;  // 这行代码会导致段错误
}

void function_b()
{
  function_c();  // 调用会触发错误的函数
}

void function_a()
{
  function_b();  // 中间调用层
}

int main()
{
  // 注册段错误信号处理器
  signal(SIGSEGV, handle_segv);

  std::cout << "程序启动，准备触发段错误...\n";
  std::cout << "按下回车键继续...\n";
  std::cin.get();

  // 调用函数链，最终会触发段错误
  function_a();

  // 正常情况下不会执行到这里
  std::cout << "程序正常结束\n";
  return 0;
}
