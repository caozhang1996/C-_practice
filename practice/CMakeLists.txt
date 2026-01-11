cmake_minimum_required(VERSION 3.20)
project(practice)

set(CMAKE_CXX_STANDARD 20)

find_package(rosa_cmake REQUIRED)
# find_package(OpenCV 4.5 REQUIRED)

rosa_find_package(
  rosa
)


# sequence_node
add_executable(sequence_node src/sequence_node.cpp)

target_include_directories(sequence_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(sequence_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  sequence_node
)

add_executable(odd_sequence_node src/odd_sequence_node.cpp)

target_include_directories(odd_sequence_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(odd_sequence_node
  PUBLIC
  rosa::rosa
)


# align_node
add_executable(align_node src/align_node.cpp)

target_include_directories(align_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(align_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  align_node
)


# sigsegv
add_executable(sigsegv src/sigsegv.cpp)

target_include_directories(sigsegv PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_options(sigsegv PRIVATE
    -g                  # 生成调试信息
    -rdynamic           # 导出符号表，用于backtrace解析函数名
)

target_link_libraries(sigsegv
  PUBLIC
  rosa::rosa
  -rdynamic
)

rosa_install_executable(
  sigsegv
)

# thread_pool
add_executable(thread_pool src/thread_pool.cpp src/main.cpp)

target_include_directories(thread_pool PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(thread_pool
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  thread_pool
)


# learning_thread
add_executable(learning_thread src/learning_thread.cpp)

target_include_directories(learning_thread PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(learning_thread
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  learning_thread
)


# jthread_node
add_executable(jthread_node src/jthread_node.cpp)

target_include_directories(jthread_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(jthread_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  jthread_node
)


# # opencv_node
# add_executable(opencv_node src/opencv_node.cpp)

# target_include_directories(opencv_node PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>
# )

# target_link_libraries(opencv_node
#   PUBLIC
#   rosa::rosa
#   ${OpenCV_LIBS}
# )

# rosa_install_executable(
#   opencv_node
# )


# mixin_node
add_executable(mixin_node src/mixin_node.cpp)

target_include_directories(mixin_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(mixin_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  mixin_node
)


# future_node
add_executable(future_node src/future_node.cpp)

target_include_directories(future_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(future_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  future_node
)


# shared_future_node
add_executable(shared_future_node src/shared_future_node.cpp)

target_include_directories(shared_future_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(shared_future_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  shared_future_node
)


# operator_node
add_executable(operator_node src/operator_node.cpp)

target_include_directories(operator_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(operator_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  operator_node
)

# string_view_node
add_executable(string_view_node src/string_view_node.cpp)

target_include_directories(string_view_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(string_view_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  string_view_node
)

# if_switch_node
add_executable(if_switch_node src/if_switch_node.cpp)

target_include_directories(if_switch_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(if_switch_node
  PUBLIC
  rosa::rosa
)

rosa_install_executable(
  if_switch_node
)


rosa_package()
