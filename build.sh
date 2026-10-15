#!/usr/bin/bash
# build.sh [project] [target]
#
# 用法：
#   ./build.sh                          编译全部（向后兼容）
#   ./build.sh practice                 只编译 practice 子项目
#   ./build.sh design_patterns          只编译设计模式
#   ./build.sh template_practice        只编译模板练习
#   ./build.sh state_machine            只编译状态机
#   ./build.sh practice sequence_node   只编译 practice 下的 sequence_node
#   ./build.sh state_machine            打印 state_machine 的 colcon 构建指引
#                                        （state_machine 已是 ROS2 ament 包，用 colcon 构建）
#
# 首次运行自动创建 build/ 目录。

set -e

BUILD_DIR="build"
PROJECT="${1:-all}"
TARGET="${2}"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 只在 CMakeLists.txt 有变化时重新 configure，否则是 no-op
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=../install \
      ..

build_project() {
    local project_name=$1
    local aggregate_target="project_${project_name}"
    echo "=========================================="
    echo "→ 编译子项目: ${project_name}"
    echo "=========================================="
    cmake --build . --target "${aggregate_target}"
}

case "$PROJECT" in
  all)
    cmake --build .
    # 不加 --component = 安装全部
    cmake --install .
    ;;
  practice|design_patterns|template_practice)
    if [ -n "$TARGET" ]; then
      echo "→ 编译 ${PROJECT} 下的单个目标: ${TARGET}"
      cmake --build . --target "${TARGET}"
      echo "→ ${TARGET} 编译完成"

      # 单目标：只装 runtime，不装头文件（调试时不需要）
      cmake --install . --component "${PROJECT}_rt" 2>/dev/null || true
    else
      build_project "${PROJECT}"
      echo "→ ${PROJECT} 编译完成"

      # 整个子项目：装 runtime + 头文件
      cmake --install . --component "${PROJECT}_rt" 2>/dev/null || true
      cmake --install . --component "${PROJECT}_dev" 2>/dev/null || true
    fi
    ;;
  state_machine)
    # state_machine 已改为 ROS2 ament 包，由 colcon 独立构建
    echo "state_machine 已是 ROS2 ament 包，请在 state_machine/ 目录内用 colcon 构建："
    echo ""
    echo "  # 依赖 ROS2 环境"
    echo "  source /opt/ros/humble/setup.bash"
    echo ""
    echo "  # 进入包目录并构建（动态库 libstate_machine.so + 测试程序）"
    echo "  cd state_machine"
    echo "  colcon build"
    echo ""
    echo "  # 生效 + 运行测试"
    echo "  source install/setup.bash"
    echo "  ros2 run state_machine lifecycle_node_test"
    ;;
  *)
    echo "用法: $0 [all|practice|design_patterns|template_practice|state_machine] [target名]"
    echo "示例:"
    echo "  $0                         # 编译全部"
    echo "  $0 practice                # 只编译 practice"
    echo "  $0 practice sequence_node  # 只编译 sequence_node"
    echo "  $0 state_machine           # 打印 colcon 构建指引（源码见 state_machine/README.md）"
    exit 1
    ;;
esac

echo ""
echo "✓ 完成"
