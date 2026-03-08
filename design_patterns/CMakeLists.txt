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

install(TARGETS crtp observer visitor state audio_player strategy mediator
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/ DESTINATION include)
