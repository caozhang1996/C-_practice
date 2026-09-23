# C++ Practice

C++ 基础知识学习与练习项目，涵盖**设计模式**、**C++ 语法与基本功**、**模板元编程**、**状态机**四大模块。

> 需要 **CMake ≥ 3.20** 和 **支持 C++20 的编译器**（GCC ≥ 10 / Clang ≥ 12 / MSVC ≥ 2022）。

## 快速开始

```bash
# 编译全部子项目
bash build.sh

# 编译某个子项目
bash build.sh practice
bash build.sh design_patterns
bash build.sh template_practice
bash build.sh state_machine

# 编译单个可执行文件
bash build.sh practice sequence_node
bash build.sh design_patterns observer
```

## 子项目

| 子项目 | 说明 | 编译 |
|--------|------|------|
| [Design Patterns](design_patterns/README.md) | GoF 23 种设计模式的 C++ 实现 | `bash build.sh design_patterns` |
| [C++ 语法与基本功](practice/README.md) | 智能指针、多线程、内存管理、C++ 新特性 | `bash build.sh practice` |
| [模板练习](template_practice/README.md) | 从基础模板到 C++20 Concepts 的系统练习 | `bash build.sh template_practice` |
| [状态机](state_machine/README.md) | ROS2 Lifecycle 风格的状态机实现 | `bash build.sh state_machine` |

## 项目结构

```
.
├── CMakeLists.txt               # 顶层 CMake 配置
├── build.sh                     # 一键构建脚本（支持按子项目选择编译）
│
├── design_patterns/             # 模块一：设计模式
│   └── README.md
├── practice/                    # 模块二：C++ 语法与基本功
│   └── README.md
├── template_practice/           # 模块三：模板练习
│   └── README.md
├── state_machine/               # 模块四：状态机
│   └── README.md
│
├── build/                       # 构建产物（自动生成）
└── install/                     # 安装目录（自动生成）
```

## 构建产物

```
install/
├── lib/
│   ├── design_patterns/        # 设计模式可执行文件
│   ├── practice/               # 语法练习可执行文件
│   ├── template_practice/      # 模板练习可执行文件
│   └── state_machine/          # 状态机可执行文件
└── include/                    # 头文件
```

## 编译器与标准

- **C++ 标准**：C++20（`practice` 和 `template_practice` 明确设置 `CMAKE_CXX_STANDARD 20`）
- **编译器**：兼容 GCC、Clang、MSVC
- **构建系统**：CMake ≥ 3.20
- **OpenCV**：可选依赖

## 许可

本项目为个人 C++ 学习实践项目，仅供学习参考。