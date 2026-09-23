# Template Practice

C++ 模板的全面练习，涵盖模板基础到 C++20 最新特性。所有测试集成在 `src/main.cpp` 中运行。

```bash
# 编译运行
bash ../build.sh template_practice
../build/template_practice/template
```

## 模板函数（Template Functions）

- **template_function_1** — 基础模板函数：`max`、`print`
- **template_function_2** — 函数模板重载与特化：`process`
- **template_function_3** — 变参模板：`printAll` 参数包展开
- **template_function_4** — 模板函数高级应用

## 模板类（Template Classes）

- **template_class_1~7** — 基础到高级的类模板，覆盖成员函数模板、友元模板、继承等
- **template_class_8** — 显式实例化（`template struct X<int>`），分离编译支持

## 模板变量（Template Variables）

- **template_variable_1** — C++14 模板变量与全特化
- **template_variable_2** — 模板变量的高级用法

## SFINAE 与类型萃取（SFINAE & Type Traits）

- **template_SFINAE** — 标准 SFINAE 实现
- **template_SFINAE_example** — SFINAE 实际应用案例
- **template_enable_if** — `std::enable_if` 对模板参数施加约束
- **template_void_t** — C++17 `std::void_t` 实现类型检测
- **is_same_v** — 自定义 `is_same` 类型 traits

## Concepts（C++20）

- **template_concept_1** — 基础 concept 定义与使用
- **template_concept_2** — concept 组合与约束
- **template_requires_1** — `requires` 表达式基础
- **template_requires_2** — requires 子句复杂约束

## 编译期编程（Compile-time Programming）

- **template_if** — `if constexpr` 编译期分支
- **template_static_member_1** — 模板类中的静态成员管理
- **template_tuple** — 变参模板实现类似 `std::tuple` 的功能
- **template_dependent_names_1~3** — 依赖名称与 `typename`/`template` 关键字

## 文件结构

```
template_practice/
├── CMakeLists.txt
├── README.md
├── include/template_practice/     # 30+ 模板头文件
│   ├── template_function_1.h
│   ├── template_function_2.h
│   ├── template_class_1.h
│   ├── template_concept_1.h
│   ├── is_same_v.h
│   └── ...
└── src/
    ├── main.cpp                   # 集成了所有模板测试的入口
    └── template_class_8.cpp       # 模板类显式实例化
```