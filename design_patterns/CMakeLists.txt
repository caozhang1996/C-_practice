cmake_minimum_required(VERSION 3.20)
project(design_patterns)

find_package(rosa_cmake REQUIRED)

# uncomment the following section in order to fill in
# further dependencies manually.
# rosa_find_package(
#  <dependency1>
#  <dependency2>
#  ...
#)

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

rosa_install_executable(
  crtp
  observer
  visitor
)


rosa_package()
