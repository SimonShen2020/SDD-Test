cmake_minimum_required(VERSION 3.15)
project(CameraSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 本地第三方库集成
add_subdirectory(thirdparty/nlohmann_json/src/json-3.11.2)
add_subdirectory(thirdparty/googletest/src/googletest-release-1.12.1)

enable_testing()
add_subdirectory(src/core)
add_subdirectory(src/plugins/huarui)
add_subdirectory(src/plugins/null)
add_subdirectory(tests)
add_subdirectory(example/ChunkData)
add_subdirectory(example/main)
