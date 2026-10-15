# State Machine

ROS2 Lifecycle 风格的状态机实现，模拟节点的状态流转（Unconfigured → Inactive → Active → Finalized）及过渡状态管理。

```bash
# 在 state_machine/ 目录内构建（动态库 libstate_machine.so + 测试程序 lifecycle_node_test）
source /opt/ros/humble/setup.bash
colcon build

# 生效后运行测试
source install/setup.bash
ros2 run state_machine lifecycle_node_test
```

> 说明：`state_machine` 是标准 **ament 包**（`package.xml` + `ament_cmake`），用 **colcon** 构建，
> 产物在包内的 `build/ install/ log/` 下。`source install/setup.bash` 会把各包的 `lib` 目录
> （含本包与 `/opt/ros/humble/lib`）加进 `LD_LIBRARY_PATH`，所以 `libstate_machine.so` 与
> rclcpp 都能被找到，不存在"找不到共享库"的问题。

## 实现细节

- **lifecycle_msgs**（ROS2 依赖） — 状态 ID 和转换 ID 直接使用 ROS2 官方的 `lifecycle_msgs::msg::State` 与 `lifecycle_msgs::msg::Transition`
- **data_types.h / data_types.cpp** — 状态机数据结构定义与实现
- **lifecycle_state_machine.h / lifecycle_state_machine.cpp** — 核心状态机逻辑，处理状态注册、转换、回调
- **state_machine**（动态库） — 由 `src/` 编译为 `libstate_machine.so`，对外导出，其他包可 `find_package(state_machine)`
- **lifecycle_node_test** — 测试入口，演示如何作为使用方创建节点并注册回调

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
├── include/
│   └── state_machine/           # 对外头文件（lifecycle.h / lifecycle_node.h ...）
├── src/
│   ├── data_types.h/.cpp        # 数据结构
│   ├── lifecycle_state_machine.h/.cpp  # 核心状态机
│   ├── lifecycle_node.*/.cpp    # 生命周期节点（对外接口）
│   └── ...                      # 编译为 libstate_machine.so
└── test/
    └── lifecycle_node_test.cpp  # 测试入口：TestNode 注册回调并驱动状态流转
```

`state_machine` 以**动态库**形式对外提供，`test/lifecycle_node_test.cpp` 作为使用方示例：
- `TestNode` 持有 `std::shared_ptr<lifecycle::LifecycleNode>`，实现 `onConfigure / onActivate / ...` 回调，并通过 `registerOnConfigure / registerOnActivate / ...` 注册