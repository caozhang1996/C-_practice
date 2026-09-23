cmake_minimum_required(VERSION 3.20)
project(design_patterns)

# crtp
add_executable(crtp src/crtp.cpp)

target_include_directories(crtp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(crtp
  PUBLIC
)

# observer
add_executable(observer src/observer.cpp)

target_include_directories(observer PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(observer
  PUBLIC
)

# visitor
add_executable(visitor src/visitor.cpp)

target_include_directories(visitor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(visitor
  PUBLIC
)

# state
add_executable(state src/state.cpp)

target_include_directories(state PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(state
  PUBLIC
)

# audio_player
add_executable(audio_player src/audio_player.cpp)

target_include_directories(audio_player PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(audio_player
  PUBLIC
)

# strategy
add_executable(strategy src/strategy.cpp)

target_include_directories(strategy PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(strategy
  PUBLIC
)

# mediator
add_executable(mediator src/mediator.cpp)

target_include_directories(mediator PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(mediator
  PUBLIC
)

# factory_method
add_executable(factory_method src/factory_method.cpp)

target_include_directories(factory_method PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(factory_method
  PUBLIC
)

# abstract_factory
add_executable(abstract_factory src/abstract_factory.cpp)

target_include_directories(abstract_factory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(abstract_factory
  PUBLIC
)

# CMake 允许为 install() 命令加一个 COMPONENT 标签，然后 cmake --install . --component <名字> 只安装这个标签的内容。

# 打个比方：


# install(TARGETS sequence_node ...  COMPONENT practice_rt)    ← 贴标签
# install(DIRECTORY include/ ...    COMPONENT practice_dev)    ← 贴标签
#                                                     ↑
# cmake --install . --component practice_rt          ← 只取 practice 的执行文件
# cmake --install . --component practice_dev          ← 只取 practice 的头文件
# 不加 --component 时，cmake --install . 安装所有组件（和原来一样）。

install(TARGETS
  crtp
  observer
  visitor
  state
  audio_player
  strategy
  mediator
  factory_method
  abstract_factory
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  COMPONENT design_patterns_rt
)

install(DIRECTORY include/ DESTINATION include
  COMPONENT design_patterns_dev
)

# 聚合 target: 供 build.sh 按子项目选择编译
# add_custom_target() — 创建一个不产生任何编译产物的"虚拟 target"`
# add_dependencies() — 声明 project_design_patterns 依赖于前面定义的 9 个可执行文件 target
# 这样形成了一个依赖链:
# project_design_patterns
#   ├── crtp
#   ├── observer
#   ├── visitor
#   ├── state
#   ├── audio_player
#   ├── strategy
#   ├── mediator
#   ├── factory_method
#   └── abstract_factory

# 三者合起来的效果

# cmake --build . --target project_design_patterns
# 执行这条命令时，CMake 会：
# 1. 发现要构建 project_design_patterns——但它是个 custom target，没有文件要生成
# 2. 查找它的依赖——发现它依赖于 crtp、observer 等 9 个 target
# 3. 递归构建所有依赖——依次检查每个依赖是否需要重新编译（源码变了就编译，没变就跳过）
# 4. 所有依赖构建完，project_design_patterns 本身无事可做，直接完成

set(DESIGN_PATTERNS_EXECUTABLES
    crtp observer visitor state audio_player strategy mediator
    factory_method abstract_factory
)
add_custom_target(project_design_patterns)
add_dependencies(project_design_patterns ${DESIGN_PATTERNS_EXECUTABLES})
