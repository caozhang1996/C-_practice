# C++ 语法与基本功

涵盖智能指针、多线程编程、内存管理、C++ 新特性等方向的练习代码。

```bash
# 编译全部
bash ../build.sh practice

# 只编译单个示例
bash ../build.sh practice sequence_node

# 运行
../build/practice/thread_pool
```

## 智能指针（Smart Pointers）

| 可执行文件 | 说明 |
|-----------|------|
| `my_shared_ptr` | 手写简易 `shared_ptr`，实现引用计数、拷贝构造/赋值、析构释放，理解 shared_ptr 核心原理 |
| `my_unique_ptr` | 手写简易 `unique_ptr`，实现移动语义、禁用拷贝、自动析构，理解 unique_ptr 所有权模型 |
| `my_make_unique` | C++11 时期自定义 `make_unique`（C++14 才引入标准版），使用变参模板和完美转发 |
| `custom_deleter` | `unique_ptr` 自定义删除器的两种方式：函数指针 vs lambda，对比二者类型擦除的差异 |
| `enable_shared_from_this` | 演示 `std::enable_shared_from_this` 的用法，解决在类内部获取 `shared_ptr` 的需求 |

## 多线程（Multithreading）

| 可执行文件 | 说明 |
|-----------|------|
| `thread_pool` | 完整线程池实现：动态调整线程数、本地任务队列、任务窃取（work-stealing）、线程亲和性。配套 `main.cpp` 提供完整测试用例。 |
| `learning_thread` | `std::invoke` 的使用：成员函数、普通函数、函数对象、lambda 的统一调用方式 |
| `jthread_node` | C++20 `std::jthread` 与 `std::stop_token`：自动 join、中断回调、协作式取消 |
| `future_node` | `std::future` 的多线程问题：多个线程从同一个 future 取值的竞争行为 |
| `shared_future_node` | `std::shared_future` 的正确用法：允许多线程并发读取同一结果 |
| `join_thread`（仅头文件） | RAII 风格的线程包装类，析构时自动 join，避免资源泄漏 |

## 内存与性能（Memory & Performance）

| 可执行文件 | 说明 |
|-----------|------|
| `align_node` | **缓存行对齐**与**伪共享（False Sharing）** 性能对比：`NonAligned` 与 `alignas(64)` 对齐结构体在多线程下的性能差异 |
| `stack_overflow` | 栈相关操作：查看进程栈限制（`getrlimit`）、制造栈溢出、运行时调整栈大小（`setrlimit`） |
| `deep_copy` | 深拷贝与 **Rule of Three**（三大法则）：拷贝构造、赋值运算符、析构函数的正确实现 |
| `virtual_table_ptr` | 空指针调用虚函数的崩溃分析——理解虚函数表指针的工作原理 |

## C++ 语法特性（Modern C++ Features）

| 可执行文件 | 说明 |
|-----------|------|
| `operator_node` | 自定义字面量运算符：`1_h`、`2.5_h` 以及 `<chrono>` 标准字面量（`10h`、`100ns`） |
| `string_view_node` | `std::string_view` 的高性能字符串视图使用与悬垂引用陷阱 |
| `if_switch_node` | C++17 `if/switch` 初始化语句：在条件中声明变量，限制变量作用域 |
| `mixin_node` | 模板 Mixin 组合模式：通过 `Compose<LoggableMixin, ResettableMixin>` 组合多个行为 |

## 模板元编程技巧（Template Metaprogramming）

| 可执行文件 | 说明 |
|-----------|------|
| `sequence_node` | 编译期生成整数序列：手写 `IndexSequence`/`MakeIndexSequence`，模拟 `std::index_sequence` |
| `odd_sequence_node` | 通过 `std::index_sequence` 和 `std::make_index_sequence` 生成奇数序列 |

## 调试与工具（Debugging）

| 可执行文件 | 说明 |
|-----------|------|
| `sigsegv` | 注册 SIGSEGV 信号处理函数，捕获段错误后打印函数调用栈（`backtrace`/`backtrace_symbols`） |
| `opencv_node` | （注释状态）演示 OpenCV `cv::Mat` 使用 `std::vector.data()` 时的野指针问题——数据生命周期管理 |

## 文件结构

```
practice/
├── CMakeLists.txt
├── README.md
├── include/practice/
│   ├── thread_pool.h          # 线程池完整实现（含模板方法）
│   └── join_thread.h          # RAII 线程包装类
└── src/
    ├── my_shared_ptr.cpp      # 手写 shared_ptr
    ├── my_unique_ptr.cpp      # 手写 unique_ptr
    ├── my_make_unique.cpp     # 自定义 make_unique（C++11）
    ├── custom_deleter.cpp     # 自定义删除器
    ├── enable_shared_from_this.cpp
    ├── thread_pool.cpp        # 线程池实现
    ├── main.cpp               # 线程池测试入口
    ├── learning_thread.cpp    # std::invoke 与线程基础
    ├── jthread_node.cpp       # C++20 std::jthread & stop_token
    ├── future_node.cpp        # std::future 多线程问题
    ├── shared_future_node.cpp # std::shared_future 多读示例
    ├── align_node.cpp         # 缓存行对齐与伪共享
    ├── stack_overflow.cpp     # 栈溢出与 rlimit
    ├── deep_copy.cpp          # 深拷贝 & Rule of Three
    ├── virtual_table_ptr.cpp  # 虚函数表指针测试
    ├── sequence_node.cpp      # 索引序列
    ├── odd_sequence_node.cpp  # 奇数序列生成
    ├── operator_node.cpp      # 自定义字面量运算符
    ├── string_view_node.cpp   # std::string_view 使用与陷阱
    ├── if_switch_node.cpp     # if/switch 初始化语句
    ├── mixin_node.cpp         # Mixin 组合模式
    ├── sigsegv.cpp            # 段错误处理与 backtrace
    └── opencv_node.cpp        # OpenCV Mat 生命周期（注释状态）
```