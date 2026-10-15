cmake_minimum_required(VERSION 3.20)
project(state_machine)

set(CMAKE_CXX_STANDARD 20)

# ROS2 依赖路径自动检测（无需手动 source setup.bash 也能 find_package）
foreach(_ros_dir /opt/ros/humble /opt/ros/iron /opt/ros/rolling /opt/ros/jazzy)
  if(EXISTS ${_ros_dir}/share/lifecycle_msgs)
    list(APPEND CMAKE_PREFIX_PATH ${_ros_dir})
    message(STATUS "Found ROS2 at ${_ros_dir}")
    break()
  endif()
endforeach()

find_package(ament_cmake REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp REQUIRED)

# ── 状态机框架：动态库 ─────────────────────────────────────
add_library(${PROJECT_NAME} SHARED
  src/data_types.cpp
  src/lifecycle.cpp
  src/lifecycle_state_machine.cpp
  src/lifecycle_interface.cpp
  src/lifecycle_node.cpp
  src/lifecycle_node_impl.cpp
)

# 声明 ROS2 依赖：include 路径 + 链接 + 传递给下游
ament_target_dependencies(${PROJECT_NAME} rclcpp lifecycle_msgs)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

# 导出给其他 ament 包 find_package(state_machine) 使用
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_targets(export_${PROJECT_NAME})
ament_export_dependencies(rclcpp lifecycle_msgs)

# ── 单元测试 ───────────────────────────────────────────────
add_executable(lifecycle_node_test
  test/lifecycle_node_test.cpp
)

ament_target_dependencies(lifecycle_node_test rclcpp)
target_link_libraries(lifecycle_node_test
  ${PROJECT_NAME}
)

# 测试程序安装到 lib/state_machine/，可通过 ros2 run state_machine lifecycle_node_test 执行
install(TARGETS lifecycle_node_test
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()