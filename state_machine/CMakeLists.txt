cmake_minimum_required(VERSION 3.20)
project(state_machine)

set(CMAKE_CXX_STANDARD 20)

# state_machine
add_executable(${PROJECT_NAME}
  src/data_types.cpp
  src/lifecycle_state_machine.cpp
  src/main.cpp
)

target_include_directories(${PROJECT_NAME}  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC 
)

install(TARGETS ${PROJECT_NAME} 
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/ DESTINATION include)

# 聚合 target：供 build.sh 按子项目选择编译
add_custom_target(project_state_machine)
add_dependencies(project_state_machine ${PROJECT_NAME})
