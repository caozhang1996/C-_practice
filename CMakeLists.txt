cmake_minimum_required(VERSION 3.20)
project(cpp_practice)

add_subdirectory(template_practice)
add_subdirectory(design_patterns)
add_subdirectory(practice)
# state_machine 已改为 ROS2 ament 包，不再参与本 superbuild，用 colcon 独立构建：
#   cd state_machine && source /opt/ros/humble/setup.bash && colcon build
