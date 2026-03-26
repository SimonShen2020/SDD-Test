cmake_minimum_required(VERSION 3.15)
project(ChunkData LANGUAGES CXX)

# 查找当前目录下的所有源文件
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 添加包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ../../SDK/05_Huarui/Include/IMV
)

# 添加库目录
target_link_directories(${PROJECT_NAME} PRIVATE
    ../../SDK/05_Huarui/Lib/x64
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    MVSDKmd.lib
)

# 添加编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _CRT_SECURE_NO_WARNINGS
    UNICODE
    _UNICODE
)

# Windows 特定设置
if(WIN32)
    if(MSVC)
        set_property(TARGET ${PROJECT_NAME} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()
