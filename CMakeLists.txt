cmake_minimum_required(VERSION 3.16)

project(CameraSDD VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CompilerOptions)
include(Dependencies)
include(Sanitizers)

option(CAMERA_BUILD_TESTS "Build camera tests" ON)

add_subdirectory(src/common)
add_subdirectory(src/core)
add_subdirectory(src/plugins/null)
add_subdirectory(src/plugins/huarui)

if(CAMERA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
