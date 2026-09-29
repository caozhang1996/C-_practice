cmake_minimum_required(VERSION 3.20)
project(state_machine)

set(CMAKE_CXX_STANDARD 20)

# ROS2 lifecycle_msgs 依赖
# 自动检测 ROS2 安装路径（无需手动 source setup.bash）
foreach(_ros_dir /opt/ros/humble /opt/ros/iron /opt/ros/rolling /opt/ros/jazzy)
  if(EXISTS ${_ros_dir}/share/lifecycle_msgs)
    list(APPEND CMAKE_PREFIX_PATH ${_ros_dir})
    message(STATUS "Found ROS2 at ${_ros_dir}")
    break()
  endif()
endforeach()
find_package(lifecycle_msgs REQUIRED)

# state_machine
add_executable(${PROJECT_NAME}
  src/data_types.cpp
  src/lifecycle_state_machine.cpp
  src/lifecycle_interface.cpp
  src/lifecycle_node.cpp
  src/lifecycle_node_impl.cpp
  src/main.cpp
)

target_include_directories(${PROJECT_NAME}  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  lifecycle_msgs::lifecycle_msgs
)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  COMPONENT state_machine_rt
)

install(DIRECTORY include/ DESTINATION include
  COMPONENT state_machine_dev
)

# 聚合 target：供 build.sh 按子项目选择编译
add_custom_target(project_state_machine)
add_dependencies(project_state_machine ${PROJECT_NAME})
