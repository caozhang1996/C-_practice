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
    msgs=("所有子项目编译完成")
    ;;
  practice|design_patterns|template_practice|state_machine)
    if [ -n "$TARGET" ]; then
      echo "→ 编译 ${PROJECT} 下的单个目标: ${TARGET}"
      cmake --build . --target "${TARGET}"
      echo "→ ${TARGET} 编译完成"
    else
      build_project "${PROJECT}"
      echo "→ ${PROJECT} 编译完成"
    fi
    ;;
  *)
    echo "用法: $0 [all|practice|design_patterns|template_practice|state_machine] [target名]"
    echo "示例:"
    echo "  $0                         # 编译全部"
    echo "  $0 practice                # 只编译 practice"
    echo "  $0 practice sequence_node  # 只编译 sequence_node"
    exit 1
    ;;
esac

# 安装（只安装已构建的目标，未构建的自动跳过）
cmake --install . 2>/dev/null || true

echo ""
echo "✓ 完成"
