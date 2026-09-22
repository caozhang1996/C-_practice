# C++ Practice

C++ 基础知识学习与练习项目，涵盖**设计模式**、**C++ 语法与基本功**、**模板元编程**三大模块。

## 目录

- [构建与运行](#构建与运行)
- [项目结构](#项目结构)
- [子模块：design_patterns（设计模式）](#子模块design_patterns设计模式)
- [子模块：practice（C++ 语法与基本功）](#子模块practicec-语法与基本功)
- [子模块：template_practice（模板练习）](#子模块template_practice模板练习)
- [编译器与标准](#编译器与标准)
- [许可](#许可)

---

## 构建与运行

> 需要 **CMake ≥ 3.20** 和 **支持 C++20 的编译器**（GCC ≥ 10 / Clang ≥ 12 / MSVC ≥ 2022）。

```bash
# 一键构建所有子模块
bash build.sh
```

`build.sh` 会：

1. 删除旧的 `build/` 目录
2. 用 CMake 生成 Debug 构建
3. 编译所有可执行文件
4. `make install` 安装到 `install/` 目录

构建产物安装在 `install/` 中，结构如下：

```
install/
├── lib/
│   ├── design_patterns/     # 设计模式可执行文件
│   ├── practice/            # 语法练习可执行文件
│   └── template_practice/   # 模板练习可执行文件
└── include/                 # 头文件
```

手动构建：

```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../install ..
cmake --build .
make install
```

运行单个示例（以观察者模式为例）：

```bash
./build/design_patterns/observer
```

运行所有线程池测试：

```bash
./build/practice/thread_pool
```

---

## 项目结构

```
C-_practice/
├── CMakeLists.txt                     # 顶层 CMake 配置（聚合三个子模块）
├── build.sh                           # 一键构建脚本
├── .gitignore                         # 构建产物忽略规则
│
├── design_patterns/                   # 模块一：设计模式
│   ├── CMakeLists.txt
│   ├── include/design_patterns/
│   │   └── audio_player.h             # 音频播放器头文件（状态模式完整实现）
│   └── src/
│       ├── abstract_factory.cpp       # 抽象工厂模式
│       ├── factory_method.cpp         # 工厂方法模式
│       ├── observer.cpp               # 观察者模式
│       ├── strategy.cpp               # 策略模式
│       ├── state.cpp                  # 状态模式
│       ├── visitor.cpp                # 访问者模式
│       ├── mediator.cpp               # 中介者模式
│       ├── crtp.cpp                   # CRTP 奇异递归模板模式
│       └── audio_player.cpp           # 音频播放器（状态模式综合应用）
│
├── practice/                          # 模块二：C++ 语法与基本功
│   ├── CMakeLists.txt
│   ├── include/practice/
│   │   ├── thread_pool.h              # 线程池完整实现（含模板方法）
│   │   └── join_thread.h              # RAII 线程包装类
│   └── src/
│       ├── my_shared_ptr.cpp          # 手写 shared_ptr
│       ├── my_unique_ptr.cpp          # 手写 unique_ptr
│       ├── my_make_unique.cpp         # 自定义 make_unique（C++11）
│       ├── custom_deleter.cpp         # 自定义删除器
│       ├── enable_shared_from_this.cpp# enable_shared_from_this 示例
│       ├── thread_pool.cpp            # 线程池实现
│       ├── main.cpp                   # 线程池测试入口
│       ├── learning_thread.cpp        # std::invoke 与线程基础
│       ├── jthread_node.cpp           # C++20 std::jthread & stop_token
│       ├── future_node.cpp            # std::future 多线程问题
│       ├── shared_future_node.cpp     # std::shared_future 多读示例
│       ├── align_node.cpp             # 缓存行对齐与伪共享
│       ├── stack_overflow.cpp         # 栈溢出与 rlimit
│       ├── deep_copy.cpp              # 深拷贝 & Rule of Three
│       ├── virtual_table_ptr.cpp      # 虚函数表指针测试
│       ├── sequence_node.cpp          # 索引序列（模拟 std::index_sequence）
│       ├── odd_sequence_node.cpp      # 奇数序列生成
│       ├── operator_node.cpp          # 自定义字面量运算符
│       ├── string_view_node.cpp       # std::string_view 使用与陷阱
│       ├── if_switch_node.cpp         # if/switch 初始化语句
│       ├── mixin_node.cpp             # Mixin 组合模式
│       ├── sigsegv.cpp                # 段错误处理与 backtrace
│       └── opencv_node.cpp            # OpenCV Mat 生命周期（注释状态）
│
├── template_practice/                 # 模块三：模板练习
│   ├── CMakeLists.txt
│   ├── include/template_practice/     # 30+ 模板头文件（见下方清单）
│   └── src/
│       ├── main.cpp                   # 集成了所有模板测试的入口
│       └── template_class_8.cpp       # 模板类显式实例化
│
├── build/                             # 构建产物（自动生成）
└── install/                           # 安装目录（自动生成）
```

---

## 子模块：design_patterns（设计模式）

基于 GoF 23 种设计模式，用 C++ 逐一实现，每个示例为独立的可执行文件。

| 可执行文件 | 模式 | 说明 |
|-----------|------|------|
| `abstract_factory` | **抽象工厂模式** | 创建一系列相关产品。示例：现代/古典风格的家具（椅子、沙发、茶几）生产线。将一个品牌家族的产品放在一个工厂中创建。 |
| `factory_method` | **工厂方法模式** | 每个具体工厂只负责创建单一产品。示例：圆形/方形积木工厂，添加新产品只需新增工厂类。 |
| `observer` | **观察者模式** | 一对多依赖关系，主题状态变化时自动通知所有观察者。使用 `std::weak_ptr` 管理观察者生命周期，避免 dangling pointer。 |
| `strategy` | **策略模式** | 同一行为有多种算法实现，客户端主动切换。示例：加/减/乘运算策略。与状态模式结构相似但意图不同——策略间平级、互不跳转。 |
| `state` | **状态模式** | 对象在不同状态下行为不同，且状态可自动流转。示例：电灯的开/关/闪烁三种状态。策略与状态的区别详见源码注释。 |
| `visitor` | **访问者模式** | 在不修改元素类的前提下，为对象结构添加新操作。示例：为 Circle/Rectangle 添加面积计算和绘制两种访问者。 |
| `mediator` | **中介者模式** | 降低多组件间的耦合，让组件通过中介者通信而非直接引用。示例：UI 对话框中的 Button/Textbox/Checkbox 通过 Dialog 中介者交互。 |
| `crtp` | **CRTP**（奇异递归模板模式） | 通过模板实现静态多态替代虚函数。示例：`ShapeBase<Derived>` 在编译期确定调用派生类的 `calculateArea()`，零运行时开销。 |
| `audio_player` | **状态模式综合应用** | 完整的音频播放器状态机：Locked（锁定）→ Ready（就绪）→ Playing（播放）三个状态，覆盖锁屏、播放/暂停、切歌等交互。 |

### 模式间关系

```
┌──────────────────────────────────────────────────────────────┐
│                     创建型模式                                │
│  抽象工厂 ── 生产一系列相关产品                                │
│  工厂方法 ── 单一产品生产，每个工厂只产一种                     │
├──────────────────────────────────────────────────────────────┤
│                     行为型模式                                │
│  观察者 ── 一对多通知                                        │
│  策略   ── 算法切换（客户端主动选）                            │
│  状态   ── 状态自动流转（内部规则驱动）                         │
│  访问者 ── 为对象结构添加新操作                                │
│  中介者 ── 组件间解耦通信                                     │
├──────────────────────────────────────────────────────────────┤
│                     模板技巧                                  │
│  CRTP   ── 编译期多态（零开销抽象）                           │
└──────────────────────────────────────────────────────────────┘
```

---

## 子模块：practice（C++ 语法与基本功）

涵盖智能指针、多线程编程、内存管理、C++ 新特性等方向的练习代码。

### 智能指针（Smart Pointers）

| 可执行文件 | 说明 |
|-----------|------|
| `my_shared_ptr` | 手写简易 `shared_ptr`，实现引用计数、拷贝构造/赋值、析构释放，理解 shared_ptr 核心原理 |
| `my_unique_ptr` | 手写简易 `unique_ptr`，实现移动语义、禁用拷贝、自动析构，理解 unique_ptr 所有权模型 |
| `my_make_unique` | C++11 时期自定义 `make_unique`（C++14 才引入标准版），使用变参模板和完美转发 |
| `custom_deleter` | `unique_ptr` 自定义删除器的两种方式：函数指针 vs lambda，对比二者类型擦除的差异 |
| `enable_shared_from_this` | 演示 `std::enable_shared_from_this` 的用法，解决在类内部获取 `shared_ptr` 的需求 |

### 多线程（Multithreading）

| 可执行文件 | 说明 |
|-----------|------|
| `thread_pool` | 完整线程池实现：动态调整线程数、本地任务队列、任务窃取（work-stealing）、线程亲和性。配套 `main.cpp` 提供完整测试用例。 |
| `learning_thread` | `std::invoke` 的使用：成员函数、普通函数、函数对象、lambda 的统一调用方式 |
| `jthread_node` | C++20 `std::jthread` 与 `std::stop_token`：自动 join、中断回调、协作式取消 |
| `future_node` | `std::future` 的多线程问题：多个线程从同一个 future 取值的竞争行为 |
| `shared_future_node` | `std::shared_future` 的正确用法：允许多线程并发读取同一结果 |
| `join_thread`（仅头文件） | RAII 风格的线程包装类，析构时自动 join，避免资源泄漏 |

### 内存与性能（Memory & Performance）

| 可执行文件 | 说明 |
|-----------|------|
| `align_node` | **缓存行对齐**与**伪共享（False Sharing）** 性能对比：`NonAligned` 与 `alignas(64)` 对齐结构体在多线程下的性能差异 |
| `stack_overflow` | 栈相关操作：查看进程栈限制（`getrlimit`）、制造栈溢出、运行时调整栈大小（`setrlimit`） |
| `deep_copy` | 深拷贝与 **Rule of Three**（三大法则）：拷贝构造、赋值运算符、析构函数的正确实现 |
| `virtual_table_ptr` | 空指针调用虚函数的崩溃分析——理解虚函数表指针的工作原理 |

### C++ 语法特性（Modern C++ Features）

| 可执行文件 | 说明 |
|-----------|------|
| `operator_node` | 自定义字面量运算符：`1_h`、`2.5_h` 以及 `<chrono>` 标准字面量（`10h`、`100ns`） |
| `string_view_node` | `std::string_view` 的高性能字符串视图使用与悬垂引用陷阱 |
| `if_switch_node` | C++17 `if/switch` 初始化语句：在条件中声明变量，限制变量作用域 |
| `mixin_node` | 模板 Mixin 组合模式：通过 `Compose<LoggableMixin, ResettableMixin>` 组合多个行为 |

### 模板元编程技巧（Template Metaprogramming）

| 可执行文件 | 说明 |
|-----------|------|
| `sequence_node` | 编译期生成整数序列：手写 `IndexSequence`/`MakeIndexSequence`，模拟 `std::index_sequence` |
| `odd_sequence_node` | 通过 `std::index_sequence` 和 `std::make_index_sequence` 生成奇数序列 |

### 调试与工具（Debugging）

| 可执行文件 | 说明 |
|-----------|------|
| `sigsegv` | 注册 SIGSEGV 信号处理函数，捕获段错误后打印函数调用栈（`backtrace`/`backtrace_symbols`） |
| `opencv_node` | （注释状态）演示 OpenCV `cv::Mat` 使用 `std::vector.data()` 时的野指针问题——数据生命周期管理 |

---

## 子模块：template_practice（模板练习）

C++ 模板的全面练习，涵盖模板基础到 C++20 最新特性。所有测试集成在 `template_practice/src/main.cpp` 中运行。

### 模板函数（Template Functions）

- **template_function_1** — 基础模板函数：`max`、`print`
- **template_function_2** — 函数模板重载与特化：`process`
- **template_function_3** — 变参模板：`printAll` 参数包展开
- **template_function_4** — 模板函数高级应用

### 模板类（Template Classes）

- **template_class_1~7** — 基础到高级的类模板，覆盖成员函数模板、友元模板、继承等
- **template_class_8** — 显式实例化（`template struct X<int>`），分离编译支持

### 模板变量（Template Variables）

- **template_variable_1** — C++14 模板变量与全特化
- **template_variable_2** — 模板变量的高级用法

### SFINAE 与类型萃取（SFINAE & Type Traits）

- **template_SFINAE** — 标准 SFINAE 实现
- **template_SFINAE_example** — SFINAE 实际应用案例
- **template_enable_if** — `std::enable_if` 对模板参数施加约束
- **template_void_t** — C++17 `std::void_t` 实现类型检测
- **is_same_v** — 自定义 `is_same` 类型 traits

### Concepts（C++20）

- **template_concept_1** — 基础 concept 定义与使用
- **template_concept_2** — concept 组合与约束
- **template_requires_1** — `requires` 表达式基础
- **template_requires_2** — requires 子句复杂约束

### 编译期编程（Compile-time Programming）

- **template_if** — `if constexpr` 编译期分支
- **template_static_member_1** — 模板类中的静态成员管理
- **template_tuple** — 变参模板实现类似 `std::tuple` 的功能
- **template_dependent_names_1~3** — 依赖名称与 `typename`/`template` 关键字

---

## 编译器与标准

- **C++ 标准**：C++20（`practice` 和 `template_practice` 子模块明确设置 `CMAKE_CXX_STANDARD 20`）
- **编译器**：兼容 GCC、Clang、MSVC
- **构建系统**：CMake ≥ 3.20
- **OpenCV**：可选依赖（`opencv_node` 默认注释状态，启用需取消 `CMakeLists.txt` 中的注释）

---

## 许可

本项目为个人 C++ 学习实践项目，仅供学习参考。