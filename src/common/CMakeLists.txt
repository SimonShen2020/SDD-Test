add_library(CameraCommon STATIC
    src/Log.cpp
    src/StringUtils.cpp
    src/FileUtils.cpp
)

target_include_directories(CameraCommon
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

camera_apply_default_compiler_options(CameraCommon)
camera_enable_sanitizers(CameraCommon)
