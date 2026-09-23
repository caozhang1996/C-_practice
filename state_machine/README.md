# State Machine

ROS2 Lifecycle 风格的状态机实现，模拟节点的状态流转（Unconfigured → Inactive → Active → Finalized）及过渡状态管理。

```bash
# 编译运行
bash ../build.sh state_machine
../build/state_machine/state_machine
```

## 实现细节

- **lifecycle_enums.h** — 状态 ID 和转换 ID 枚举，遵循 ROS2 Lifecycle 规范
- **data_types.h / data_types.cpp** — 状态机数据结构定义与实现
- **lifecycle_state_machine.h / lifecycle_state_machine.cpp** — 核心状态机逻辑，处理状态注册、转换、回调
- **main.cpp** — 演示状态机构建与状态流转

## 状态流转

```
PRIMARY_STATE_UNKNOWN
        │
        ▼
PRIMARY_STATE_UNCONFIGURED
        │
        ▼
PRIMARY_STATE_INACTIVE
        │
        ▼
PRIMARY_STATE_ACTIVE
        │
        ▼
PRIMARY_STATE_FINALIZED
```

## 文件结构

```
state_machine/
├── CMakeLists.txt
├── README.md
└── src/
    ├── lifecycle_enums.h              # 状态/转换 ID 枚举
    ├── data_types.h / data_types.cpp  # 数据结构
    ├── lifecycle_state_machine.h / .cpp  # 核心状态机
    └── main.cpp                       # 演示入口
```