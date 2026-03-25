add_library(CameraHuarui SHARED
    src/CameraHuaruiStub.cpp
    src/HuaruiSdkAdapter.cpp
    src/HuaruiCamera.cpp
    src/HuaruiPluginEntry.cpp
)

target_include_directories(CameraHuarui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CameraHuarui
    PRIVATE
        CameraCore
)

camera_apply_default_compiler_options(CameraHuarui)
camera_enable_sanitizers(CameraHuarui)

set_target_properties(CameraHuarui PROPERTIES
    OUTPUT_NAME "CameraHuarui"
)
