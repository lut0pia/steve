cmake_minimum_required(VERSION 3.8..3.10)

project(steve)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

add_executable(steve ${SOURCES})

# Visual Studio debugger working directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dbg)
set_target_properties(
  steve PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dbg
  CXX_STANDARD 14
)

install(TARGETS steve)
install(DIRECTORY cfg DESTINATION steve)
