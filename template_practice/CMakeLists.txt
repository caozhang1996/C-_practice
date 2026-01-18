cmake_minimum_required(VERSION 3.20)
project(template_practice)

set(CMAKE_CXX_STANDARD 20)

add_executable(template src/main.cpp)

target_include_directories(template PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(template
  PUBLIC
)

install(TARGETS template
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/ DESTINATION include)
